package student.surname.name.project2;

import java.awt.Color;
import java.util.LinkedList;

import drawing_framework.*;

/**
 * Snake in Classic Snake Game.
 * Represented by a linked list.
 * Can also be added to an AnimationCanvas.
 * @author cantunca
 */
public class Snake implements Animatable {

	SnakeGame game;

	LinkedList<SnakeUnit> body = new LinkedList<SnakeUnit>();

	private int frameCounter;
	private int moveFrameInterval;

	private Direction direction;
	private boolean isAlive;

	/**
	 * Constructor.
	 * @param game Game that the snake belongs to.
	 * @param moveFrameInterval Snake will move once per moveFrameInterval frames
	 */
	public Snake(SnakeGame game, int moveFrameInterval) {
		this.game = game;

		// Initial snake position
		body.addLast(new SnakeUnit(5, 4));
		body.addLast(new SnakeUnit(4, 4));
		body.addLast(new SnakeUnit(3, 4));
		body.addLast(new SnakeUnit(2, 4));

		frameCounter = 0;
		this.moveFrameInterval = moveFrameInterval;

		direction = getCurrentDirection();
		isAlive = true;
	}

	/**
	 * Set the direction of the snake.
	 * The movement in the given direction occurs on the next move() call.
	 * Direction is set only if it is not opposite to the current direction.
	 * @param dir Snake direction to be set
	 */
	public void setDirection(Direction dir) {
		Direction currentDir = getCurrentDirection();
		if (!((dir == Direction.UP && currentDir == Direction.DOWN)
				|| (dir == Direction.DOWN && currentDir == Direction.UP)
				|| (dir == Direction.LEFT && currentDir == Direction.RIGHT)
				|| (dir == Direction.RIGHT && currentDir == Direction.LEFT))) {
			direction = dir;
		}
	}

	/**
	 * Get the current direction of the snake.
	 * @return Current direction
	 */
	private Direction getCurrentDirection() {
		SnakeUnit head = body.getFirst();
		SnakeUnit second = body.get(1);

		int deltaX = head.getX() - second.getX();
		int deltaY = head.getY() - second.getY();
		if (deltaX == 1) {
			return Direction.RIGHT;
		} else if (deltaX == -1) {
			return Direction.LEFT;
		} else if (deltaY == -1) {
			return Direction.DOWN;
		} else {
			return Direction.UP;
		}
	}

	/**
	 * Check if the snake coincides with the given location.
	 * @param x X grid coordinate
	 * @param y Y grid coordinate
	 * @return true if snake body contains location, false otherwise
	 */
	public boolean containsLocation(int x, int y) {
		for (SnakeUnit unit:body) {
			if (unit.getX() == x && unit.getY() == y) {
				return true;
			}
		}
		return false;
	}
	
	/**
	 * Checks if the given new head collides with the snake body.
	 * This is one of the end game conditions.
	 * @param newHead SnakeUnit object representing the new head location
	 * @return true if collision occurs, false otherwise
	 */
	private boolean doesCollide(SnakeUnit newHead) {
		for (SnakeUnit unit:body) {
			if (unit == body.getLast()) {
				// ignore collision with tail, since it will certainly move out of the way
				continue;
			}
			if (unit.getX() == newHead.getX() && unit.getY() == newHead.getY()) {
				return true;
			}
		}
		return false;
	}

	@Override
	public void move(AnimationCanvas canvas) {

		// Do nothing if snake is dead
		if (!isAlive) {
			return;
		}

		// Snake is moved only once per moveFrameInterval number of frames
		frameCounter++;
		if (frameCounter < moveFrameInterval) {
			return;
		}
		frameCounter = 0;

		// Determine new head location
		SnakeUnit head = body.getFirst();
		SnakeUnit newHead = head.clone();
		newHead.moveInDirection(direction);

		// Check if snake collides with itself
		if (doesCollide(newHead)) {
			isAlive = false;
			return;
		}
		
		// Check if out of boundaries
		if (newHead.getX()<0 || newHead.getX()>=canvas.getGridWidth()
				|| newHead.getY()<0 || newHead.getY()>=canvas.getGridHeight()) {
			isAlive = false;
			return;
		}
		
		// Grow or just move
		if (game.isFoodInLocation(newHead.getX(), newHead.getY())) {
			body.addFirst(newHead);
			game.consumeFood();
		} else {
			body.addFirst(newHead);
			body.removeLast();
		}
	}

	@Override
	public void draw(AnimationCanvas canvas) {

		// Draw snake's body
		for (SnakeUnit unit:body) {
			if (unit == body.getFirst()) {
				canvas.fillGridSquare(unit.getX(), unit.getY(), Color.RED);
			} else {
				canvas.fillGridSquare(unit.getX(), unit.getY(), Color.BLACK);
			}
		}

		// Display "GAME OVER" if snake is dead
		if (!isAlive) {
			canvas.drawText("GAME OVER", canvas.getGridWidth()/2-1, canvas.getGridHeight()/2, Color.RED);
			canvas.drawText("Press R to play again", canvas.getGridWidth()/2-2, canvas.getGridHeight()/2-1, Color.RED);
		}
	}

}
