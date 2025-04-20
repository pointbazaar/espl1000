#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "tac/tac.h"
#include "basic_block/basicblock.h"
#include "tac/tacbuffer.h"

#include "liveness.h"

struct Liveness {
	// map (statement index) -> (defines set)
	// the set of temporaries defined (written to)
	// in this statement
	bool** map_defines;

	// map (statement index) -> (use set)
	bool** map_use;

	// map (statement index) -> (in set)
	bool** map_in;

	// map (statement index) -> (out set)
	// map_out[stmt_index][i] == true
	// means that at 'stmt_index', temporary 'i' is
	// live on an incoming edge
	bool** map_out;

	// a temporary t is live at stmt n, if
	// out[n][t] || in[n][t]

	// map (statement index) -> successor statements
	// map_succ[stmt_index][i] == true
	// means that there is a forward edge from
	// stmt at 'stmt_index' to stmt at 'i'
	bool** map_succ;

	// count all statements in the graph
	// populated by constructor
	size_t nstmts;

	// count all temporary variables (IR) in the graph
	size_t ntemps;
};

static void map_print(bool** map, size_t nrows, size_t ncols, char* name) {

	assert(map != NULL);

	printf("-------------\n");
	printf("%s:\n", name);

	for (size_t row = 0; row < nrows; row++) {
		printf("%ld: ", row);
		bool* x = map[row];
		for (size_t col = 0; col < ncols; col++) {
			if (x[col]) {
				printf("%ld,", col);
			}
		}
		printf("\n");
	}
}

void liveness_print(struct Liveness* l) {

	assert(l != NULL);

	map_print(l->map_defines, l->nstmts, l->ntemps, "def[n]");
	map_print(l->map_use, l->nstmts, l->ntemps, "use[n]");
	map_print(l->map_in, l->nstmts, l->ntemps, "in[n]");
	map_print(l->map_out, l->nstmts, l->ntemps, "out[n]");
}

size_t liveness_ntemps(struct Liveness* l) {
	return l->ntemps;
}

static bool liveness_get(struct Liveness* l, size_t stmt_index, size_t tmp_index) {
	return l->map_out[stmt_index][tmp_index] || l->map_in[stmt_index][tmp_index];
}

static size_t liveness_max_temp_block(struct BasicBlock* block) {
	assert(block != NULL);

	size_t max_temp = 0;
	for (size_t i = 0; i < tacbuffer_count(block->buffer); i++) {
		struct TAC* t = tacbuffer_get(block->buffer, i);

		assert(t != NULL);

		const int32_t opt_max_temp = tac_max_temp(t);

		if (opt_max_temp > (int32_t)max_temp) {
			max_temp = opt_max_temp;
		}
	}

	return max_temp;
}

static size_t liveness_count_temps(struct BasicBlock** graph, size_t nblocks) {

	assert(graph != NULL);

	size_t max_temp = 0;
	for (size_t i = 0; i < nblocks; i++) {
		size_t max_temp_block = liveness_max_temp_block(graph[i]);
		if (max_temp_block > max_temp) {
			max_temp = max_temp_block;
		}
	}

	// temporaries are allocated continuously,
	// so the highest number already defines the count.
	// e.g. {t0, t1, t2} -> 3 temporaries

	return max_temp + 1;
}

static size_t liveness_count_statements(struct BasicBlock** graph, size_t nblocks) {

	uint32_t count = 0;
	for (size_t i = 0; i < nblocks; i++) {
		count += tacbuffer_count(graph[i]->buffer);
	}
	return count;
}

static bool** map_ctor(size_t nstmts, size_t ntemps) {

	bool** res = malloc(sizeof(bool*) * nstmts);
	assert(res != NULL);
	for (size_t i = 0; i < nstmts; i++) {
		// 0-initialized
		res[i] = calloc(1, ntemps);
	}
	return res;
}

static void map_dtor(bool** map, size_t nstmts) {

	for (size_t i = 0; i < nstmts; i++) {
		free(map[i]);
	}
	free(map);
}

static struct TAC* get_stmt_from_index(struct BasicBlock** graph, size_t nblocks, uint32_t index) {

	for (size_t i = 0; i < nblocks; i++) {
		struct BasicBlock* block = graph[i];
		if (index < tacbuffer_count(block->buffer)) {
			return tacbuffer_get(block->buffer, index);
		} else {
			index -= tacbuffer_count(block->buffer);
		}
	}

	return NULL;
}

static ssize_t get_stmt_index_from_tac(struct BasicBlock** graph, size_t nblocks, struct TAC* tac) {

	size_t prev_count = 0;
	for (size_t i = 0; i < nblocks; i++) {
		struct BasicBlock* block = graph[i];
		struct TACBuffer* buf = block->buffer;

		ssize_t index = tacbuffer_indexof(buf, tac);

		if (index >= 0) {
			return prev_count + index;
		}

		const size_t count = tacbuffer_count(buf);
		prev_count += count;
	}

	return -1;
}

// @returns   true if there was a change
static bool liveness_iteration_stmt(struct Liveness* l, size_t n) {

	bool change = false;

	// in[n] = use[n] u (out[n] - def[n])

	bool* usen = l->map_use[n];
	bool* outn = l->map_out[n];
	bool* defn = l->map_defines[n];
	bool* inn = l->map_in[n];
	for (size_t index = 0; index < l->ntemps; index++) {
		const bool old = inn[index];

		inn[index] = usen[index] || (outn[index] && !defn[index]);

		change |= old != inn[index];
	}

	// out[n] = U (for s \in succ[n]) in[s]
	for (size_t index = 0; index < l->ntemps; index++) {
		for (size_t s = 0; s < l->nstmts; s++) {
			if (!l->map_succ[n][s]) {
				continue;
			}
			const bool old = outn[index];

			// now: stmt s \in succ[n]
			outn[index] |= l->map_in[s][index];
			change |= old != outn[index];
		}
	}

	return change;
}

// @returns   true if there was a change
static bool liveness_iteration(struct Liveness* l) {

	bool change = false;
	for (size_t n = 0; n < l->nstmts; n++) {

		change |= liveness_iteration_stmt(l, n);
	}

	return change;
}

static void liveness_calc_succ_block(struct Liveness* l, struct BasicBlock** graph, size_t nblocks, struct BasicBlock* block, size_t* prev_count) {
	struct TACBuffer* buf = block->buffer;
	const size_t count = tacbuffer_count(buf);

	for (size_t j = 0; j < count - 1; j++) {
		size_t stmt_index = *prev_count + j;
		l->map_succ[stmt_index][stmt_index + 1] = true;
		// consider branches to other basic blocks
		struct BasicBlock* b1 = block->branch_1;
		struct BasicBlock* b2 = block->branch_2;

		if (b1 != NULL) {
			struct TAC* first = tacbuffer_get(b1->buffer, 0);
			const size_t index = get_stmt_index_from_tac(graph, nblocks, first);
			l->map_succ[stmt_index][index] = true;
		}
		if (b2 != NULL) {
			struct TAC* first = tacbuffer_get(b2->buffer, 0);
			const size_t index = get_stmt_index_from_tac(graph, nblocks, first);
			l->map_succ[stmt_index][index] = true;
		}
	}

	*prev_count += count;
}

static void liveness_calc_succ(struct Liveness* l, struct BasicBlock** graph, size_t nblocks) {

	size_t prev_count = 0;
	for (size_t i = 0; i < nblocks; i++) {
		struct BasicBlock* block = graph[i];

		liveness_calc_succ_block(l, graph, nblocks, block, &prev_count);
	}
}

static void liveness_calc_use(struct Liveness* l, struct BasicBlock** graph, size_t nblocks) {
	for (size_t n = 0; n < l->nstmts; n++) {

		struct TAC* t = get_stmt_from_index(graph, nblocks, n);
		tac_mark_used(t, l->map_use[n], l->ntemps);
	}
}
static void liveness_calc_defines(struct Liveness* l, struct BasicBlock** graph, size_t nblocks) {
	for (size_t n = 0; n < l->nstmts; n++) {

		struct TAC* t = get_stmt_from_index(graph, nblocks, n);
		tac_mark_defines(t, l->map_defines[n], l->ntemps);
	}
}

struct Liveness* liveness_calc_tacbuffer(struct TACBuffer* buf) {
	assert(buf != NULL);

	size_t nblocks = 1;
	struct BasicBlock** graph = malloc(sizeof(struct BasicBlock*) * 1);
	struct BasicBlock* block = malloc(sizeof(struct BasicBlock));
	graph[0] = block;

	block->index = 0;
	block->buffer = buf;
	block->branch_1 = NULL;
	block->branch_2 = NULL;

	struct Liveness* live = liveness_calc(graph, nblocks);

	free(block);
	free(graph);

	return live;
}

void liveness_dtor(struct Liveness* live) {

	const size_t nstmts = live->nstmts;

	map_dtor(live->map_defines, nstmts);
	map_dtor(live->map_use, nstmts);
	map_dtor(live->map_in, nstmts);
	map_dtor(live->map_out, nstmts);
	map_dtor(live->map_succ, nstmts);

	free(live);
}

struct Liveness* liveness_calc(struct BasicBlock** graph, size_t nblocks) {

	assert(graph != NULL);
	assert(nblocks > 0);

	struct Liveness* res = malloc(sizeof(struct Liveness));
	// find total number of temporaries
	res->ntemps = liveness_count_temps(graph, nblocks);
	assert(res->ntemps > 0);

	// find total number of IR statements (struct TAC instances)
	res->nstmts = liveness_count_statements(graph, nblocks);
	assert(res->nstmts > 0);

	// The order of IR statements in our table is defined by the order given by the array where they are stored and
	// the order in which their BasicBlocks are stored.

	// create the table mapping each IR statement
	// to in, out sets, and liveness

	res->map_defines = map_ctor(res->nstmts, res->ntemps);
	res->map_use = map_ctor(res->nstmts, res->ntemps);
	res->map_in = map_ctor(res->nstmts, res->ntemps);
	res->map_out = map_ctor(res->nstmts, res->ntemps);

	res->map_succ = map_ctor(res->nstmts, res->nstmts);

	liveness_calc_succ(res, graph, nblocks);
	liveness_calc_use(res, graph, nblocks);
	liveness_calc_defines(res, graph, nblocks);

	// calculate in,out,...
	while (true) {
		bool change = liveness_iteration(res);

		if (!change) {
			break;
		}
	}

	return res;
}

// 2 temporaries cannot share the same register if they
// are live at some point in the program
//
// @returns  true    if both temporaries are live
//                   at some statement
// @returns  false   otherwise
bool liveness_overlaps(struct Liveness* l, uint32_t temp1, uint32_t temp2) {
	for (size_t i = 0; i < l->nstmts; i++) {
		bool live1 = liveness_get(l, i, temp1);
		bool live2 = liveness_get(l, i, temp2);
		if (live1 && live2) {
			return true;
		}
	}
	return false;
}

bool liveness_use(struct Liveness* live, uint32_t stmt_index, uint32_t temp) {

	if (stmt_index >= live->nstmts) {
		fprintf(stderr, "stmt %ld out of bounds\n", live->nstmts);
		assert(false);
	}

	return live->map_use[stmt_index][temp];
}

bool liveness_def(struct Liveness* live, uint32_t stmt_index, uint32_t temp) {

	if (stmt_index >= live->nstmts) {
		fprintf(stderr, "stmt %ld out of bounds\n", live->nstmts);
		assert(false);
	}

	return live->map_defines[stmt_index][temp];
}

bool liveness_in(struct Liveness* live, uint32_t stmt_index, uint32_t temp) {

	if (stmt_index >= live->nstmts) {
		fprintf(stderr, "stmt %ld out of bounds\n", live->nstmts);
		assert(false);
	}

	return live->map_in[stmt_index][temp];
}

bool liveness_out(struct Liveness* live, uint32_t stmt_index, uint32_t temp) {

	if (stmt_index >= live->nstmts) {
		fprintf(stderr, "stmt %ld out of bounds\n", live->nstmts);
		assert(false);
	}
	if (temp >= live->ntemps) {
		fprintf(stderr, "temp %ld out of bounds\n", live->ntemps);
		assert(false);
	}

	return live->map_out[stmt_index][temp];
}
