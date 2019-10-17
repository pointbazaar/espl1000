public namespace FacultyJava{
	public static void main(String[] args){
		System.out.println(faculty(3));
	}

	public static int faculty(int n){
		if(n==1){
			return 1;
		}else{
			return n*faculty(n-1);
		}
	}
}
