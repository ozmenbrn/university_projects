package student.surname.name.project2;

import java.awt.Color;

import drawing_framework.*;

/**
 * Food in Classic Snake Game.
 * Basically a square.
 * @author cantunca
 */
public class Food implements Animatable {

	private int x, y;
	
	public Food() {
		x = 0;
		y = 0;
	}
	
	public int getX() {
		return x;
	}

	public void setX(int x) {
		this.x = x;
	}

	public int getY() {
		return y;
	}

	public void setY(int y) {
		this.y = y;
	}

	@Override
	public void move(AnimationCanvas canvas) {
		// Do nothing
	}

	@Override
	public void draw(AnimationCanvas canvas) {
		canvas.fillGridSquare(x, y, Color.ORANGE);
	}

}
