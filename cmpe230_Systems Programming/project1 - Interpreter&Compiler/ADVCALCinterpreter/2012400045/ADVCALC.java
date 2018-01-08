
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Scanner;
import java.util.Stack;

public class ADVCALC {
   private Stack theStack;		//for postfix operations
   private String input;					// ""
   private String output = "";				// ""
   
   /**
    * Default constructor 
    * @param input takes String as an input and create a stack to write postfix 
    */
   public ADVCALC(String input) {
      this.input = input;
      theStack = new Stack<>();
   }
   
   public static <E> void main(String[] args){  
	   ADVCALC theTrans = new ADVCALC("");
	   @SuppressWarnings("resource")
	   Scanner console = new Scanner(System.in);
	   String str= "";
	   
	   boolean equal= false;	// Look that the input has equal sign or not
	   int isThere = 0;			// for different variables.
	   int vari=0;				// for different variables.
	 
	   ArrayList<String> list = new ArrayList<String>(); // This arraylist is for variable names ex.("x")
	   
	   String[] strArr = new String[1];

	   int[] variable = new int[52];
	   
	   /**
	    * We know how many lines in there, So we can look step by step with for loop
	    * This for loop is for to know how many different variables are there like x, y, z...
	    * So, if we know how many variables we can make an array of integers that corresponds with the same number of index with arraylist
	    */
	   while(true){
	   for(int i=0; i<1 ; i++){
		   strArr[i] = console.nextLine();
		   
		   for(int j=0; j<strArr[i].length(); j++){
			   if((strArr[i].charAt(j) >=65 && strArr[i].charAt(j) <=90) || (strArr[i].charAt(j) >=97 && strArr[i].charAt(j) <=122) ){
				   if(list.isEmpty()){
					   list.add(strArr[i].substring(j, j+1));
					   vari++;
				   }
				   for(int b =0; b<list.size(); b++){
					   if(list.get(b).equals(strArr[i].substring(j, j+1))){
						   isThere ++;
					   }
				   }	
				   if(isThere==0){
					   list.add(strArr[i].substring(j, j+1));
					   vari ++;
				   }
				   isThere = 0;
			   }
		   }
	   }
	   	   
	       //By knowing how many variable THE array can be decleared
	   
	   /**
	    * first looks for "=" sign for the operation. If line input has equal sign than "=" operation is done
	    * Otherwise, print the result that variables have.
	    */
	   for(int i=0; i<1 ; i++){
		   int t = 0;
		   int l = 0;
		   for(int j=0; j<strArr[i].length(); j++){
			   t++;
			   if(strArr[i].charAt(j) == 61){
				   equal = true;
				   l=t;
			   }
		   }	
		   for(int k=0; k<strArr[i].length(); k++){
			  if(equal){	  
				  str = strArr[i].substring(l);
				  
				  theTrans = new ADVCALC(str);
				  str = theTrans.doTrans();
				  
				  for(int a=0; a<list.size(); a++){
					  if(list.get(a).equals(strArr[i].substring(0,1))){
						  variable[a] = result(str, list, variable);
					  }
				  }
			  }
			  else{
				  str = strArr[i].substring(0);
				  
				  theTrans = new ADVCALC(str);
				  str = theTrans.doTrans();
				  
				  System.out.println(result(str, list, variable));
				  break;
			  }
		   }
		   
		   equal = false;
	   }
	   } 
   }
   
   /**
    * 
    * @param str that comes from main in postfix form to make able to calculate
    * @param list is combined with the array of integer and keeps the values's id
    * @param variable	is combined with the list of values's id. So it can return integer value
    * @return the result of postfix operation if it is both equal sign or operation
    */
   private static int result(String str, ArrayList<String> list, int[] variable){
	   int result = 0;
	   int count = 0;
	   String temp ="";
	   Stack<String> integers = new Stack<String>();
	   LinkedList<String> operators = new LinkedList<String>();
	   
	   //Looks for a tab or space
	   for(int i=0; i<str.length() ; i++){
		   if(str.charAt(i) == 9 || str.charAt(i) == 32){
		//	   System.out.println(str);
		   }
		   //look for an operation (+,*,/,-) and do the operation
		   else if(str.charAt(i)>=40  && str.charAt(i) <=47){
			   operators.add(str.substring(i,i+1));
			   
			   if(operators.getFirst().equals("+")){
				   result = Integer.valueOf((String) integers.pop()) + Integer.valueOf((String) integers.pop());
				   temp += result;
				   integers.push(temp);
				   operators.removeFirst();
				   temp ="";
			   }
			   else if(operators.getFirst().equals("-")){
				   int tempo = Integer.valueOf((String) integers.pop());
				   result = Integer.valueOf((String) integers.pop()) - tempo;
				   temp += result;
				   integers.push(temp);
				   operators.removeFirst();
				   temp ="";
			   }
			   else if(operators.getFirst().equals("*")){
				   result = Integer.valueOf((String) integers.pop()) * Integer.valueOf((String) integers.pop());
				   temp += result;
				   integers.push(temp);
				   operators.removeFirst();
				   temp ="";
			   }
			   else{
				   int tempi = Integer.valueOf((String) integers.pop());
				   result =  Integer.valueOf((String) integers.pop()) / tempi;
				   temp += result;
				   integers.push(temp);
				   operators.removeFirst();
				   temp ="";
			   }		   
		   }
		   // if the string has a name like x, y push it to stack with is integer value
		   // if the string has integer value push it to the stack
		   else{
			   if(str.charAt(i)<=122 && str.charAt(i) >= 65){
				   for(int b=0; b<list.size(); b++){
					   if(str.substring(i,i+1).equals(list.get(b))){
						   temp += variable[b];
						   integers.push(temp);
						   count++;
					   }
				   }
			   }
			   else{
				   int look = 1;
				   int tempI = i;
				   while(str.charAt(tempI+1) >= 48 && str.charAt(tempI+1) <= 57){
		       		 	 look++;
		       		 	 tempI++;
				   }
				   if(str.charAt(tempI+1) >=40 && str.charAt(tempI+1)<=47){
					   integers.push(str.substring(i, i+1));
				   }
				   else{
					   integers.push(str.substring(i,i+look));
					   count++;	
					   for(int p=1; p<look; p++){
						   i++;
					   }	   
				   }
				   
			   }
		   }
		   
		   temp ="";
	   }	
	   if(count == 1){
		   result = Integer.valueOf((String) integers.pop());
	   }    
	   return result;   
   }
   
   /**
    * do the operation that takes infix and
    * @return postfix
    */
   public String doTrans() {
	   input = input + " ";
	      for (int j = 0; j < input.length(); j++) {
	    	 String ch ="";
	         if(input.charAt(j) >= 48 && input.charAt(j) <= 57){
	        	 ch = input.substring(j, j+1);
	        	 while(input.charAt(j+1) >= 48 && input.charAt(j+1) <= 57){
	        		 j++;
	        		 ch += input.substring(j,j+1);	 
	        	 }
	         } 
	         else{
	        	 ch = input.substring(j, j+1);
	         }
	         switch (ch) {
	            case "+": 
	            case "-":
	            gotOper(ch, 1); 
	            break; 
	            case "*": 
	            case "/":
	            gotOper(ch, 2); 
	            break; 
	            case "(": 
	            theStack.push(ch);
	            break;
	            case ")": 
	            gotParen(ch); 
	            break;
	            default: 
	            output = output + ch; 
	            break;
	         }
	      }
	      
	      while (!theStack.isEmpty()) {
	         output = output + theStack.pop();
	      }
	      
	      return output; 
   }

   public void gotOper(String opThis, int prec1) {
      while (!theStack.isEmpty()) {
         String opTop = (String) theStack.pop();
         if (opTop.equals("(")) {
            theStack.push(opTop);
            break;
         }
         else {
            int prec2;
            if (opTop.equals("+") || opTop.equals("-"))
            prec2 = 1;
            else
            prec2 = 2;
            if (prec2 < prec1) { 
               theStack.push(opTop);
               break;
            }
		    else
            output = output + opTop;
         }
      }
      theStack.push(opThis);
   }
   
   
   public void gotParen(String ch){ 
      while (!theStack.isEmpty()) {
         String chx = (String) theStack.pop();
         if (chx.equals("(")) 
	         break; 
	         else
	         output = output + chx; 
	       
	      }
	   }
  }
