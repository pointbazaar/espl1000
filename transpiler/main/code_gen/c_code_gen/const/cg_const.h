#ifndef C_CODE_GEN_CONST_H
#define C_CODE_GEN_CONST_H


//const nodes related
void transpileBoolConst		(struct BoolConst* bc, 	struct Ctx* ctx);
void transpileIntConst		(struct IntConst* ic, 	struct Ctx* ctx);
void transpileHexConst		(struct HexConst* hc, 	struct Ctx* ctx);
void transpileBinConst		(struct BinConst* hc, 	struct Ctx* ctx);
void transpileCharConst		(struct CharConst* cc, 	struct Ctx* ctx);
void transpileFloatConst	(struct FloatConst* fc, struct Ctx* ctx);
void transpileStringConst	(struct StringConst* s, struct Ctx* ctx);

#endif
