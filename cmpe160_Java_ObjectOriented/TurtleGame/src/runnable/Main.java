package runnable;

import java.util.ArrayList;
import java.util.Random;
import java.util.Scanner;

import visualization.Bus;
import visualization.Car;
import visualization.Vehicle;
import visualization.Board;

public class Main {
	
	/**
	 * This is the main code.
	 * @param args
	 */
	public static void main(String[] args) {
		
		@SuppressWarnings("resource")
		Scanner console = new Scanner(System.in);
		boolean isTrue= true;
		
		Board board = new Board("OzmenBarýn", 1300, 1000);
		ArrayList<Integer> lane1 = new ArrayList<Integer>();
		ArrayList<Integer> lane2 = new ArrayList<Integer>();
		ArrayList<Integer> lane3 = new ArrayList<Integer>();
		ArrayList<Integer> lane4 = new ArrayList<Integer>();
		ArrayList<String> highScores = new ArrayList<String>();
		
		ArrayList<Vehicle> vehicles = new ArrayList<Vehicle>();
		
			/**
			 * Continue code while the frog aren't hit by the car. 
			 */
			while(isTrue){
				if(!checkFrog(board, vehicles)){
					System.out.println("GAME OVER!\n Please enter your name: ");
					board.waitFor(1000);
					board.gameOver();
					highScores.add(console.next()+" : "+board.getScorenum());
					System.out.println(highScores);
					isTrue = false;
				}
				else{
					createVehicle(board, vehicles, lane1, lane2, lane3, lane4);
		
					checkVehicleCollision(vehicles);										
					moveVehicles(vehicles, board, lane1, lane2, lane3, lane4);
					
					UpdateScore(board);
	    		}				
				
			}
		
		}
		
	

	/**
	 * 
	 * Move each vehicle on the array list one step in their corresponding direction.
	 * Change the speed of the vehicles as rounds increases.
	 * Empty the array lists lane1,lane2,lane3,lane4 as vehicles get out of the screen.
	 *
	 * @param veh visible vehicles
	 * @param board	visible board
	 * @param lane1	visible array list
	 * @param lane2 visible array list
	 * @param lane3 visible array list
	 * @param lane4 visible array list
	 */
	private static void moveVehicles(ArrayList<Vehicle> veh, Board board, ArrayList<Integer> lane1, ArrayList<Integer> lane2, ArrayList<Integer> lane3, ArrayList<Integer> lane4) {
	int currentLine;	
	double currentSpeed = 0.002;
	int currentRound = board.getRoundnum();
	if(currentRound<=1)
		currentSpeed = 0.0002;
	else if(currentRound <= 3)
		currentSpeed= 0.0004;
	else if(currentRound <= 5)
		currentSpeed= 0.0008;
	else if(currentRound <= 7)
		currentSpeed = 0.0012;
	else if(currentRound <=9)
		currentSpeed = 0.0016;
	

		if(!veh.isEmpty()){	
			for(int i=0; i<veh.size(); i++){
				if(veh.get(i).direction){
					veh.get(i).move(currentSpeed, 0);
					if(veh.get(i).getX()>1150&&veh.get(i).direction){
						currentLine= veh.get(i).getLane();
						board.remove(veh.get(i));				
						veh.remove(i);
						if(currentLine == 1 && !lane1.isEmpty()){
							lane1.remove(0);
						}
						if(currentLine == 2 && !lane2.isEmpty()){
							lane2.remove(0);
						}
						if(currentLine == 3 && !lane3.isEmpty()){
							lane3.remove(0);
						}
						if(currentLine == 4 && !lane4.isEmpty()){
							lane4.remove(0);
						}
					}
				}
				else{
					veh.get(i).move(-1*currentSpeed, 0);
					if(veh.get(i).getX()<-1150 && !veh.get(i).direction){						
						currentLine= veh.get(i).getLane();
						board.remove(veh.get(i));
						veh.remove(i);
						if(currentLine == 1 && !lane1.isEmpty()){
							lane1.remove(0);
						}
						if(currentLine == 2 && !lane2.isEmpty()){
							lane2.remove(0);
						}
						if(currentLine == 3 && !lane3.isEmpty()){
							lane3.remove(0);
						}
						if(currentLine == 4 && !lane4.isEmpty()){
							lane4.remove(0);
						}
					}
				}
			}		
		} 				
	}

	
	/**
	 * Couldn't do in time!
	 * 
	 * @param veh visible vehicles
	 * @return direction of vehicles 
	 */
	private static boolean checkVehicleCollision(ArrayList<Vehicle> veh){
		boolean isTrue = true;
//		for(int i = 0; i < veh.size(); i++){
//			for(int j =0; j<veh.size(); j++){
//				if(!veh.get(i).equals(veh.get(j))){
//					if(veh.get(i).getPosY() == veh.get(j).getPosY() && veh.get(i).getPosX()==0 && veh.get(j).getPosX() ==1100){
//						if(veh.get(i).getX()+250 ==  1100 - Math.abs(veh.get(j).getX())){
//							if(veh.get(i).getDirection() && !veh.get(j).getDirection()){
//								veh.get(i).setDirection(false);
//								veh.get(j).setDirection(true);
//							}
//						}
//					}
//				}
//			}
//		}
		return isTrue;
	}
	

	/**
	 * Check every vehicle locations and frog location and decide a vehicle hit a frog or not
	 * 
	 * @param board visible board
	 * @param veh visible vehicles
	 * @return true or false if the frog is hit by the vehicle or not
	 */
	private static boolean checkFrog(Board board,  ArrayList<Vehicle> veh){
		double turtleLeft= board.getTurtleX();
		double turtleRight= board.getTurtleX() + 70;
		double turtleTop= board.getTurtleY();
		double turtleBot= board.getTurtleY()+90;
		boolean isTrue = true;
		
		
		for(int i =0; i<veh.size(); i++){
		if(veh.get(i).getPosX()==0){	
			if(((Math.abs(turtleLeft - (veh.get(i).getX()+200) + turtleLeft - veh.get(i).getX())<200
					&&Math.abs(turtleTop - (veh.get(i).getPosY()+100)+turtleTop -veh.get(i).getPosY())<100))
					||((Math.abs(turtleLeft - veh.get(i).getX()+turtleLeft-(veh.get(i).getX()+200))<200
					&&Math.abs(turtleBot - veh.get(i).getPosY()+turtleBot-(veh.get(i).getPosY()+100))<100))){
				isTrue = false;
			}
			if(((Math.abs(turtleRight - veh.get(i).getX()+turtleRight - (veh.get(i).getX()+200))<200)
					&&(Math.abs(turtleTop - (veh.get(i).getPosY()+100)+turtleTop - veh.get(i).getPosY())<100))
					||((Math.abs(turtleRight - veh.get(i).getX()+turtleRight-(veh.get(i).getX()+200))<200)
					&&(Math.abs(turtleBot - veh.get(i).getPosY()+turtleBot-(veh.get(i).getPosY()+100))<100))){
				isTrue = false;
			}
		}
		if(veh.get(i).getPosX()==1100){	
			if(((Math.abs(turtleLeft - Math.abs(1100+(veh.get(i).getX()+200)) + turtleLeft - Math.abs(1100+veh.get(i).getX()))<200
					&&Math.abs(turtleTop - (veh.get(i).getPosY()+100)+turtleTop -veh.get(i).getPosY())<100))
					||((Math.abs(turtleLeft - Math.abs(1100+veh.get(i).getX())+turtleLeft-Math.abs(1100+(veh.get(i).getX()+200)))<200
					&&Math.abs(turtleBot - veh.get(i).getPosY()+turtleBot-(veh.get(i).getPosY()+100))<100))){
				isTrue = false;
			}
			if(((Math.abs(turtleRight - Math.abs(1100+veh.get(i).getX())+turtleRight - Math.abs(1100+(veh.get(i).getX()+200)))<200)
					&&(Math.abs(turtleTop - (veh.get(i).getPosY()+100)+turtleTop - veh.get(i).getPosY())<100))
					||((Math.abs(turtleRight - Math.abs(1100+veh.get(i).getX())+turtleRight-Math.abs(1100+(veh.get(i).getX()+200)))<200)
					&&(Math.abs(turtleBot - veh.get(i).getPosY()+turtleBot-(veh.get(i).getPosY()+100))<100))){
				isTrue = false;
			}
		}
		}
		return isTrue;
	}
	
	/**
	 * Updates scores and replace the frog's position to safe zone.
	 * 
	 * @param board visible board
	 */
	private static void UpdateScore(Board board){
		if(board.getTurtleY()<0){
			board.setScorenum(board.getScorenum()+ board.getRoundnum());
			board.setRoundnum(board.getRoundnum()+1);
			board.removeTurtle();
			board.addTurtle();
			board.removeGameInfoLabels();
			board.addGameInfoLabels();
		}	
		
	}
	

	/**
	 * Create new vehicle checking the already created vehicles.
	 * Creation, type and lane of the vehicle are decided randomly.
	 * 
	 * Once the vehicle created, if the lane is not available (occupied with some other vehicle),
	 * the newly created one is discarded.
	 * 
	 * @param board visible board
	 * @param vehicles visible vehicles
	 * @param lane1 visible lane1
	 * @param lane2	visible lane2
	 * @param lane3 visible lane3
	 * @param lane4 visible lane4
	 * @return vehicle to be added to the board
	 */
	private static Vehicle createVehicle(Board board, ArrayList<Vehicle> vehicles, ArrayList<Integer> lane1, ArrayList<Integer> lane2, ArrayList<Integer> lane3, ArrayList<Integer> lane4) {
		Random rand = new Random();
		Vehicle randomVehicle = new Car(0,0);
		int lane = randomVehicle.getLane();
		int pos = randomVehicle.setPosition();
		int createNew = rand.nextInt(100000);
		int i1 =0, i2=0, i3=0, i4=0;

			if(createNew<1 ){
				if(lane==1 && lane1.size()<2){
					randomVehicle = new Car(pos,lane*200-100);
					randomVehicle.setPosY(lane*200-100);
					randomVehicle.setPosX(pos);
					board.addObject(randomVehicle);
					randomVehicle.sendToFront();
					lane1.add(i1);
					i1++;
					vehicles.add(randomVehicle);
					if(lane1.size() == 2 && Math.abs(randomVehicle.getX())< 300 ){
						board.remove(randomVehicle);
						i1--;
						lane1.remove(i1);
						vehicles.remove(randomVehicle);
					} 
				}	
				if(lane==2 && lane2.size()<2){
					randomVehicle = new Car(pos,lane*200-100);
					board.addObject(randomVehicle);
					randomVehicle.setPosY(lane*200-100);
					randomVehicle.setPosX(pos);
					randomVehicle.sendToFront();
					lane2.add(i2);
					i2++;
					vehicles.add(randomVehicle);
					if(lane2.size() == 2 && Math.abs(randomVehicle.getX())< 300){
						board.remove(randomVehicle);
						i2--;
						lane2.remove(i2);
						vehicles.remove(randomVehicle);
					}
				}
				if(lane==3 && lane3.size()<2){
					randomVehicle = new Car(pos,lane*200-100);
					board.addObject(randomVehicle);
					randomVehicle.setPosY(lane*200-100);
					randomVehicle.setPosX(pos);
					randomVehicle.sendToFront();
					lane3.add(i3);
					i3++;
					vehicles.add(randomVehicle);
					if(lane3.size() == 2 && Math.abs(randomVehicle.getX())< 300){
						board.remove(randomVehicle);
						i3--;
						lane3.remove(i3);
						vehicles.remove(randomVehicle);
					}
				}
				if(lane==4 && lane4.size()<2){
					randomVehicle = new Car(pos,lane*200-100);
					randomVehicle.setPosY(lane*200-100);
					randomVehicle.setPosX(pos);
					board.addObject(randomVehicle);
					randomVehicle.sendToFront();
					lane4.add(i4);
					i4++;
					vehicles.add(randomVehicle);
					if(lane4.size() == 2 && Math.abs(randomVehicle.getX())< 300){
						board.remove(randomVehicle);
						i4--;
						lane4.remove(i4);
						vehicles.remove(randomVehicle);
					}
				}
				
			}
			if(createNew>99998){
				if(lane==1 && lane1.size()<2){
					randomVehicle = new Bus(pos-50,lane*200-100);
					randomVehicle.setPosY(lane*200-100);
					randomVehicle.setPosX(pos);
					board.addObject(randomVehicle);
					randomVehicle.sendToFront();
					lane1.add(i1);
					i1++;
					vehicles.add(randomVehicle);
					if(lane1.size() == 2 && Math.abs(randomVehicle.getX())< 400){
						board.remove(randomVehicle);
						i1--;
						lane1.remove(i1);
						vehicles.remove(randomVehicle);
					} 
				}	
				if(lane==2 && lane2.size()<2){
					randomVehicle = new Bus(pos-50,lane*200-100);
					randomVehicle.setPosY(lane*200-100);
					randomVehicle.setPosX(pos);
					board.addObject(randomVehicle);
					randomVehicle.sendToFront();
					lane2.add(i2);
					i2++;
					vehicles.add(randomVehicle);
					if(lane2.size() == 2 && Math.abs(randomVehicle.getX())< 400){
						board.remove(randomVehicle);
						i2--;
						lane2.remove(i2);
						vehicles.remove(randomVehicle);
					}
				}	
				if(lane==3 && lane3.size()<2){
					randomVehicle = new Bus(pos-50,lane*200-100);
					randomVehicle.setPosY(lane*200-100);
					randomVehicle.setPosX(pos);
					board.addObject(randomVehicle);
					randomVehicle.sendToFront();
					lane3.add(i3);
					i3++;
					vehicles.add(randomVehicle);
					if(lane3.size() == 2 &&Math.abs(randomVehicle.getX())< 400){
						board.remove(randomVehicle);
						i3--;
						lane3.remove(i3);
						vehicles.remove(randomVehicle);
					}
				}
				
				if(lane==4 && lane4.size()<2){
					randomVehicle = new Bus(pos-50,lane*200-100);
					randomVehicle.setPosY(lane*200-100);
					randomVehicle.setPosX(pos);
					board.addObject(randomVehicle);
					randomVehicle.sendToFront();
					lane4.add(i4);
					i4++;
					vehicles.add(randomVehicle);
					if(lane4.size() == 2 && Math.abs(randomVehicle.getX())< 400){
						board.remove(randomVehicle);
						i4--;
						lane4.remove(i4);
						vehicles.remove(randomVehicle);
					}
				}	
				
			}
		
		return randomVehicle;
	}
}