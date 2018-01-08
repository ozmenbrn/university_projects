package visualization;

import java.awt.Color;
import java.awt.Font;

import acm.graphics.GLabel;
import acm.graphics.GOval;
import acm.graphics.GRect;

@SuppressWarnings("serial")
public class Car extends Vehicle {
	private GRect[] windows;
	private GOval[] wheels;
	private GRect body;
	private GLabel label;

	
	
	public Car(int objX, int objY) {
		super(objX, objY);
		this.width = 200;
		this.height = 100;
		this.windowLength = 25;
		this.wheelCircle = 20;
		addBody(objX, objY);
		addWindows(objX, objY);
		addWheels(objX, objY);
		addLabel(objX, objY);
		if(objX==0){
			direction = true;
		}else if(objX==1100){
			direction= false;
		}
	}

	@Override
	public void addBody(int objX, int objY) {
		body = new GRect(width, height);
		body.setFillColor(Color.BLUE);
		body.setFilled(true); 
		add(body, objX, objY);
		
	}

	@Override
	public void addWindows(int objX, int objY) {
		windows = new GRect[2];
		for (int i=0; i<2; i++) {
			windows[i] = new GRect(windowLength,windowLength);
			windows[i].setFillColor(Color.WHITE);
			windows[i].setFilled(true); 
		}
		add(windows[0], objX+(width-2*windowLength)/3, objY+10);
		add(windows[1], objX+windowLength+2*(width-2*windowLength)/3, objY+10);
		
	}

	@Override
	public void addWheels(int objX, int objY) {
		wheels = new GOval[2];
		for (int i=0; i<2; i++) {
			wheels[i] = new GOval(wheelCircle,wheelCircle);
			wheels[i].setFillColor(Color.RED);
			wheels[i].setFilled(true); 
		}
		add(wheels[0], objX+(width-2*wheelCircle)/3, objY+height-10);
		add(wheels[1], objX+wheelCircle+2*(width-2*wheelCircle)/3, objY+height-10);
		
	}

	@Override
	public void addLabel(int objX, int objY) {
		label = new GLabel("Barýn Turizm");
		label.setFont(new Font("Arial", Font.BOLD, 18));
		label.setColor(Color.WHITE);
		add(label,objX+45,objY+50);
		
	}
	

}
