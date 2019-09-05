package org.vanautrui.languages.dragoncompiler.parsing.astnodes.nonterminal.upperscopes;

import org.vanautrui.languages.dragoncompiler.lexing.utils.TokenList;
import org.vanautrui.languages.dragoncompiler.parsing.IASTNode;

import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashSet;
import java.util.Set;
import java.util.stream.Collectors;

public class AST implements IASTNode {

    public Set<ClassNode> classNodeList = new HashSet<>();

    public Path srcPath= Paths.get("/dev/null");

    public AST(){}

    public AST(TokenList tokens, Path path) throws Exception {
        this.srcPath=path;

        TokenList copy = new TokenList(tokens);

        boolean success_class = true;
        while (success_class) {

            try {
                this.classNodeList.add(new ClassNode(copy,this.srcPath));
            } catch (Exception e) {

                if (this.classNodeList.size() == 0) {
                    throw e;
                }
                success_class = false;
            }
        }
    }

    @Override
    public String toSourceCode() {
        return this.classNodeList
                .stream()
                .map(node -> node.toSourceCode())
                .collect(Collectors.joining("\n"));
    }
}
