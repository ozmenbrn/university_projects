package visualization;

import java.awt.Color;
import java.awt.Font;

import acm.graphics.GLabel;
import acm.graphics.GOval;
import acm.graphics.GRect;

@SuppressWarnings("serial")
public class Bus extends Vehicle{
	
	private GRect[] windows;
	private GOval[] wheels;
	private GRect body;
	private GLabel label;
	
	public Bus(int objX, int objY) {
		super(objX, objY);
		this.width = 300;
		this.height = 150;
		this.windowLength = 50;
		this.wheelCircle = 30;
		addBody(objX, objY-80);
		addWindows(objX, objY-80);
		addWheels(objX, objY-80);
		label.sendToFront();
		if(objX==-50){
			direction = true;
		}else if(objX==1050){
			direction= false;
		}
		
	}

	@Override
	public void addBody(int objX, int objY) {
		body = new GRect(width, height);
		body.setFillColor(Color.RED);
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
			wheels[i].setFillColor(Color.BLACK);
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
		add(label,objX+90,objY+10);
		label.sendToFront();
		
	}

}
