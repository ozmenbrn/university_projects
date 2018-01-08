import java.io.*;    //to reach file
import java.util.*;  //to reach scanner.

public class BÖ2012400045{
   public static void main(String[] args) throws FileNotFoundException{    //erase the error.
   Scanner input=new Scanner(new File("data.txt"));   //by writing input. we can reach the file.
      int sum=0;  //identified a sum for using it later.
      while(input.hasNext()){    //every token is counting while the file has.
         input.next();  //pass the token.
         sum++;   //so we now how many tokens are there.
      } 
      input.close();       //to open file again this code should be used.
      input=new Scanner(new File("data.txt"));  //we open the file again.
      
      String[] word= new String[sum+1];   //word identifed as an array.
      while(input.hasNext()){    //same process like number 8.
         for(int i=0; i<sum; i++){
            word[i]=input.next();   //every word[0 , 1 ,2 ,3..] are identifed each by each.
         }
      }
      String elli=("");
      int count=0;      //these variables need to be identifed to use it later.
      int total=0;
      for(int a=0; a<sum; a++){
         String t=word[a];    
         total=0;   
         for(int i=0; i<sum; i++){
            String b=word[i];       
            if(t.equalsIgnoreCase(b)){    //comparing 2 of all words and if they are equal, total is adding up by 1. 
               total++;
            }
         }
         if(total>sum/2){  //if total is more than sum/2, it means a word is used more than%50.
            count++;
            elli=t;
         }
      }
      if(count>0){   //by quiting for loop the we know the exact valuse of count. 
         System.out.println("This program is spam due to "+elli+" is appeared more than %50.");
      }
      
      
         total=0;
         
      for(int i=0; i<sum; i++){
         if(word[i].equalsIgnoreCase("apartment")){   //looking every word[i] if it is equal to apartment or not. if it equals more than one so it is spam.
            total++;
         }
      }   
         if(total>1){
            System.out.println("apartment is appeared more than once.");
         }
         total=0;
      
      for(int i=0; i<sum; i++){
         if(word[i].equalsIgnoreCase("sale")){   //looking every word[i] if it is equal to sale or not. if it equals more than one so it is spam.
            total++;
         }
      }  
         if(total>1){
            System.out.println("sale is appeared more than once.");
         }
         total=0;
      
      for(int i=0; i<sum; i++){
         if(word[i].equalsIgnoreCase("rent")){   //looking every word[i] if it is equal to rent or not. if it equals more than one so it is spam.
            total++;
         }
      }
         if(total>1){
            System.out.println("rent is appeared more than once.");
         }
         total=0;
                                                       
      for(int i=0; i<sum; i++){
         if(word[i].equalsIgnoreCase("money")){   //looking every word[i] if it is equal to money or not. if it equals more than one so it is spam.
            total++;
         }
      }
         if(total>1){
            System.out.println("money is appeared more than once.");
         }
         total=0;
                     
      for(int i=0; i<sum; i++){
         if(word[i].equalsIgnoreCase("bank")){   //looking every word[i] if it is equal to bank or not. if it equals more than one so it is spam.
            total++;
         }
      }
         if(total>1){
            System.out.println("bank is appeared more than once.");
         }
      
      String k=("");
      String j=(" and ");     //these variables are also defined to use later again.
      String hey=("");
      count=0;             //we reset the count. it equals 0 againg.
      for(int i=0; i<sum; i++){
         String y=word[i];
         total=0;   
         for(int a=0; i<sum; i++){
            String z=word[a];
            if(y.equals(z)){  //again it counts words and if any word appears more than %30 we can get it from total.
               total++;
            }
         hey=y;
         }
         if(total>(sum*3)/10){   //if total is more than %30. count is added by 1.
            count++; 
            k=hey+j;
            
         }
      }
      if(count>2){      //if count is more than 2. we know that, there are at least 3 words that appear more than %90 total and it is spam.
         System.out.println(k+" are appeared %30 each.");
      }
      
      count=0;    //reseting count.
      String o=("");    //again variables.
      String p=("");
      String hop=("");
      for(int i=0; i<sum; i++){
         o=word[i];
         p=word[i+1];            //by writing p=(word +1) we know 2 words which are consecutively and if they are these 4 words below which are consecutive, so it is spam.
         if(o.equalsIgnoreCase("dear")|| o.equalsIgnoreCase("sir")|| o.equalsIgnoreCase("madam")|| o.equalsIgnoreCase("honorable")){
            if(p.equalsIgnoreCase("dear")|| p.equalsIgnoreCase("sir")|| p.equalsIgnoreCase("madam")|| p.equalsIgnoreCase("honorable")){
               count++;
               hop=(o+" and "+p);
            }
         }
      }
      if(count>0){
         System.out.println("This file is spam due to "+hop+" are consecutively.");
      }
      
      
   }
}