public class Experiment{
	public static void main(String[] args){
	
		A a=new A();
		a.x.foo();
		a.x.foo().x;
	}
}

class A{
	A x;
	public A foo(){
		return new A();
	}
}
