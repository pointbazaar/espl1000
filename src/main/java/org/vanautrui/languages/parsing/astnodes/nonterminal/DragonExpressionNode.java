package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.IDragonTermNode;
import org.vanautrui.languages.parsing.astnodes.IExpressionComputable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonIntegerConstantNode;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.Set;

public class DragonExpressionNode implements IDragonASTNode, IExpressionComputable, IDragonTermNode {

    //DragonExpressionNode should be similar to jack expression
    //an expression should be anything that returns a value or computes to a value

    public DragonTermNode term;

    public List<DragonOperatorNode> operatorNodes=new ArrayList<>();

    public List<DragonTermNode> termNodes=new ArrayList<>();

    public DragonExpressionNode(DragonTokenList tokens) throws Exception {

        DragonTokenList copy = tokens.copy();

        this.term=new DragonTermNode(copy);

        try{
            DragonTokenList copy2=new DragonTokenList(copy);
            while(true){
                DragonOperatorNode myop=new DragonOperatorNode(copy2);
                DragonTermNode myterm=new DragonTermNode(copy2);

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
    public void doTypeCheck(Set<DragonAST> asts, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) throws Exception {
        //check that the terms have a type such that the operator will work

        //for later:
        //int * string      //repeat the string x times

        //string + string   //concatenate
        //int + int
        //int - int
        //int * int
        //int / int

        //expression is compiled to evaluate in order. meaning
        //that if there are 3 terms,
        //the first gets evaluated, then the second, then the operator gets applied
        //such for convienience we check for now that the terms have all the same type

        //as of now i think we should first focus on integer addition
        //and let the other cases throw an exception
        //they should be implemented later

        for(DragonOperatorNode op : this.operatorNodes){
            if(!op.operator.equals("+")){
                throw new Exception("only '+' is supported for now");
            }
        }

        for (DragonTermNode t : this.termNodes){
            if(!(t.termNode instanceof DragonIntegerConstantNode)){
                throw new Exception("only integers are supported for now. the other stuff shall follow later");
            }
        }

        //TODO: look for the other cases
    }

    @Override
    public String toSourceCode() {
        StringBuilder res = new StringBuilder(term.toSourceCode());

        for(int i=0;i<this.operatorNodes.size();i++){
            DragonOperatorNode o=this.operatorNodes.get(i);
            DragonTermNode t=this.termNodes.get(i);
            res.append(" ").append(o.toSourceCode()).append(" ").append(t.toSourceCode()).append(" ");
        }

        return res.toString();
    }

    @Override
    public String getType(DragonMethodNode methodNode) throws Exception {
        //TODO: for now it just returs 'Int' but it should work in the other cases too
        //return "Int";

        //it should only return anything, when all the types are the same
        //because for now lets keep it simple.
        //otherwise it should throw an exception

        String type = this.term.getType(methodNode);

        /*
        for(DragonOperatorNode op : this.operatorNodes){
            if(!op.operator.equals("+")){
                throw new Exception("only '+' is supported for now");
            }
        }

         */

        for (DragonTermNode t : this.termNodes){
            if(!(t.termNode.getType(methodNode).equals(type))){
                throw new Exception("the types are not the same, "+type+" collides with "+t.termNode.getType(methodNode));
            }
        }

        return this.term.getType(methodNode);
    }
}
