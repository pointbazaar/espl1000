package org.vanautrui.languages.dragoncompiler.parsing.astnodes.nonterminal;

import org.vanautrui.languages.dragoncompiler.lexing.utils.TokenList;
import org.vanautrui.languages.dragoncompiler.parsing.IASTNode;
import org.vanautrui.languages.dragoncompiler.parsing.astnodes.ITermNode;
import org.vanautrui.languages.dragoncompiler.parsing.astnodes.IExpressionComputable;

import java.util.ArrayList;
import java.util.List;

public class ExpressionNode implements IASTNode, IExpressionComputable, ITermNode {

    //DragonExpressionNode should be similar to jack expression
    //an expression should be anything that returns a value or computes to a value

    public TermNode term;

    public List<OperatorNode> operatorNodes=new ArrayList<>();

    public List<TermNode> termNodes=new ArrayList<>();

    public ExpressionNode(TokenList tokens) throws Exception {

        TokenList copy = tokens.copy();

        this.term=new TermNode(copy);

        try{

            while(true){
                TokenList copy2=new TokenList(copy);

                OperatorNode myop=new OperatorNode(copy2);
                TermNode myterm=new TermNode(copy2);

                this.operatorNodes.add(myop);
                this.termNodes.add(myterm);

                copy.set(copy2);
            }
        }catch (Exception e){
            //pass
        }

        tokens.set(copy);
    }


    @Override
    public String toSourceCode() {
        StringBuilder res = new StringBuilder(term.toSourceCode());

        for(int i=0;i<this.operatorNodes.size();i++){
            OperatorNode o=this.operatorNodes.get(i);
            TermNode t=this.termNodes.get(i);
            res.append(" ").append(o.toSourceCode()).append(" ").append(t.toSourceCode()).append(" ");
        }

        return res.toString();
    }
}
