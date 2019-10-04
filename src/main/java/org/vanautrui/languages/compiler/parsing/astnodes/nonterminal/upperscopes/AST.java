package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes;

import org.apache.commons.lang3.tuple.Pair;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.IASTNode;

import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashSet;
import java.util.Set;
import java.util.stream.Collectors;

public class AST implements IASTNode {

    public Set<NamespaceNode> namespaceNodeList = new HashSet<>();

    public Path srcPath= Paths.get("/dev/null");

    public AST(){}

    public AST(TokenList tokens, Path path,boolean debug) throws Exception {
        this.srcPath=path;

        TokenList copy = new TokenList(tokens);

        //just pass the tokens needed for this class
        Pair<TokenList, TokenList> pair = copy.split_into_tokens_and_next_block_and_later_tokens();

        boolean success_class = true;
        while (success_class) {

            try {
                int chunk_size = pair.getLeft().size();
                this.namespaceNodeList.add(new NamespaceNode(pair.getLeft(),this.srcPath,debug));
                copy.consume(chunk_size);
                pair = pair.getRight().split_into_tokens_and_next_block_and_later_tokens();
            } catch (Exception e) {

                if (this.namespaceNodeList.size() == 0) {
                    throw e;
                }
                success_class = false;
            }
        }

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return this.namespaceNodeList
                .stream()
                .map(node -> node.toSourceCode())
                .collect(Collectors.joining("\n"));
    }
}
