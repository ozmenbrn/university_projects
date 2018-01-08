import java.util.*;         //so that i can use user's input on this program and also it allows me to use random.
public class Projectt2{          //Project's name.
   public static void main(String[] args){      //main class
   Scanner console= new Scanner(System.in);     //With import java.util.* and this one we can use users inputs.
   Random rand=new Random();     //It is for using random answers.
      int a;      //I will need a variable in whole code, so i defined one named a.
      String answer4=(" ");      //I defined a variable answer4 and use it in down.
      while(answer4.equals("y")|| answer4.equals(" ")){  //by user's answer it goes in while loop. If it is correct answer, it will go on, but if it is not, the question will be asked again until it is true due to "while". 
      System.out.println("Hello, what is your name?"); // asking a question.
      String name=console.next();   //User's answer will be our "name" variable.
      System.out.println("Hello "+name);  //Hello + user's input.
      System.out.println("What is your age?");  
      int age=console.nextInt(); //User's input will be our "age" variable.
         if(age<=25){   //if age is under 25, the code will follow this road.
            System.out.println("Are you a student?");
            String answer=console.next();
            while(!answer.equals("yes")&&!answer.equals("no")){  //if the answer is not equal either yes or no the question will be asked again.
               System.out.println("That is not a valid answer.");
               answer=console.next();
            }                                                 
               if(answer.equals("yes")){  //after saying yes another road will be folowed.
                  System.out.println("What is your GPA?");
                  double gpa=console.nextDouble(); //gpa will be defined by user.
                  while(gpa<0 || gpa>4){  //if the gpa is not realistic like -10 or 20, the the question will be asked again.
                     System.out.println("That is not a valid answer");
                     gpa=console.nextInt();
                  }
                     if(gpa<2){  //if it is under the 2.
                        a=rand.nextInt(3);   //our "a"variable will be useful in here. So that i can use random method.
                        if(a==0){   
                        System.out.println("Loser!");
                        }
                        else if(a==1){
                        System.out.println("Hope you are having fun");     //in whole if(a==1)(a==0)... is randomly selected.
                        }
                        else{
                        System.out.println("So,you are a failing student");
                        }
                     }
                     else if(gpa<2.5){ //if it is between 2-2,5
                        System.out.println("You need to work harder");
                     }
                     else{ //if it is between 2,5-4.
                        a=rand.nextInt(2);   //again we use "a" variable and change it, and use it for again random.
                        if(a==0){
                        System.out.println("Good job");
                        }                                            //it is again answer will be selected randomly.
                        else{
                        System.out.println("You can do even better");
                        }
                     }
                }
                else{   //if the user says no, this sentence will be showned and the program will ask do you want to play again.
                  System.out.println("You should go to school.");
                }             
         }
            else if(age<=60){ //if age is between 25-60 this road will be fallowed.
               System.out.println("Are you working?");
               String answer2=console.next();
               while(!answer2.equals("yes")&&!answer2.equals("no")){    //again the same processes with while loop.
                  System.out.println("That is not a valid answer.");
                  answer2=console.next();          
               }
                  if(answer2.equals("yes")){
                     System.out.println("What is your occupation?");
                     String occupation=console.next();      //i get occupation from user to use it after.
                     System.out.println("What is your salary?");
                     int salary=console.nextInt();    //i get salary from user to use it after again.
                     
                     if(!occupation.equals("teacher")&& !occupation.equals("cook")&& !occupation.equals("driver")&& !occupation.equals("computerengineer")&& !occupation.equals("journalist")){
                        a=rand.nextInt(5);      //in #70 i am checking if the user's occupation input and these 5 occupations are same or not. if not, so the program will go to#71 to#87.
                        if(a==0){
                           System.out.println("If you were a cook, you could have earned "+(salary*2));
                        }
                        else if(a==1){
                           System.out.println("If you were a driver, you could have earned "+(salary*2));
                        }
                        else if(a==2){
                           System.out.println("If you were a Computer Engineer, you could have earned "+(salary*2));   //these are all for picked random answers.
                        }
                        else if(a==3){
                           System.out.println("If you were a journalist, you could have earned "+(salary*2));
                        }
                        else{
                           System.out.println("If you were a teacher, you could have earned "+(salary*2));
                        }
                     }
                     else{    //if the user's occupation and one of these 5 occupation are same, so user need an answer. i give it this one in #89.
                        System.out.println("Have a good day :)");
                     }
                  }
                  else{ //if the person do not have a job, so, the program will print the sentence in down.
                     System.out.println("You should find a job first.");
                  }
    
            }
            else{ //if the user's age is over 60, this road is being fallowed.
               System.out.println("Are you retired?");
               String answer3=console.next();
                  while(!answer3.equals("yes")&&!answer3.equals("no")){    //same while processes.
                     System.out.println("That is not a valid answer.");
                     answer3=console.next();
                  }
                     if(answer3.equals("yes")){
                        a=rand.nextInt(2);
                        if(a==0){
                           System.out.println("i suggest you move to Florida");     //if the user is retired, randomly to sentences will be printed.
                        }
                        else{
                           System.out.println("i suggest you move to Antalya");
                        }
                     }
                     else{
                        System.out.println("You will be retired soon, hang on"); //if not retired, the sentences will be like this.
                     }
            }     
               System.out.println("Do you want to play again?(y/n)");   //after all question are answered this one is shown up. If she says "y", the program will be restarted, if not the program will be finished.
               answer4=console.next();
         while(!answer4.equals("y")&& !answer4.equals("n")){
            System.out.println("That is not a valid answer."); //again the same while process.
            answer4=console.next();
         } 
      } 
   }
}