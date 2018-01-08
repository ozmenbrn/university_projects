package visualization;

import java.util.Random;

import acm.graphics.GCompound;
import acm.graphics.GLabel;
import acm.graphics.GOval;
import acm.graphics.GRect;

@SuppressWarnings("serial")
public abstract class Vehicle extends GCompound{
	Random rand = new Random();

	protected GRect body;
	protected GRect[] windows;
	protected GOval[] wheels;
	protected GLabel label;
	
	protected int width, height, windowLength, wheelCircle;
	protected int lane;
	protected int position ;
	public boolean direction;
	protected int posY;
	protected int posX;
	
	
	
	public Vehicle(int objX, int objY) {
		addBody(objX, objY);
		addWindows(objX, objY);
		addWheels(objX, objY);
		addLabel(objX, objY);
	}

	
//	public boolean getDirection() {
//		return direction;
//	}
//
//
//	public void setDirection(boolean direction) {
//		this.direction = direction;
//	}


	public abstract void addBody(int objX, int objY);
	
	public abstract void addWindows(int objX, int objY);

	public abstract void addWheels(int objX, int objY);

	public abstract void addLabel(int objX, int objY);
	

	
	
	public int setPosition(){
		int whdirection = rand.nextInt(2);
		if(whdirection == 1){
			position = 1100;
			
		}
		else{
			position = 0;
			
		}
		return position;
	}
	
	
	public int getLane() {
		this.lane = rand.nextInt(4)+1;
		return lane;
	}
	
	public int getPosX() {
		return posX;
	}

	public void setPosX(int posX) {
		this.posX = posX;
	}
	
	public int getPosY() {
		return posY;
	}

	public void setPosY(int posY) {
		this.posY = posY;
	}
	
}