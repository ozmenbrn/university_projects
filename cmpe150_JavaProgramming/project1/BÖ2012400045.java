public class projectt1 {
   public static final int SIZE=3;    //this constant connect with the methods and it controls the system.
      public static void main(String[] args){
     
         fibo();     //first part of the project, also represents the first method.
         System.out.println();
         factorial(); //second part of the project's method.
         
      }    
         public static void fibo(){  //first method.
            int a=1;  //with sum and a, i can find the seri.
            int sum=2;
         
               for(int round=0; round<SIZE; round++){ // gives us the line number. for ex, if size 5, there is 5 lines.          
                  for(int x=0; x<=round; x++){     //it seperate the numbers as in the project. first line there is 1 number, in third line there are 3 numbers...
                     System.out.print(sum-a+" ");  
                     a=sum-a;                       //all 3 of these help me in fibonacci numbers.
                     sum=a+sum;    
                  }
                  for(int space=SIZE-1; space>0; space--){  //this for loop is not necesarry, just for knowing there are spaces.
                  System.out.print(" ");
                  } 
                  System.out.println();  //drop the line to the one down.
               }
         }
      
         public static void factorial(){  //second part, second method.
            int n= 1;   //the number n is changable, it only for identifies the "n".
            int x= 0; 
            
               for(int i=SIZE; i>=1; i--){ //like in first part, by this we know how many loop we have.
                  n=n*i;        
               } 
               for (int i=n; i>0; i=i/10){ //by dividing 10 and getting mod, we can plus every digits of the numbers.
                  x += i%10;  
               }               
                  System.out.println(SIZE+" factorial is "+n+ " and the sum of its digits is "+x);
         } 
}