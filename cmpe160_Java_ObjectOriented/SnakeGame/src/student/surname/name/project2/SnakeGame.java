package student.surname.name.project2;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import drawing_framework.*;

enum Direction { UP, DOWN, LEFT, RIGHT }

/**
 * Classic Snake Game.
 * Use arrow keys to navigate.
 * Press R to restart game.
 * You must provide the GUI that the game will draw itself on.
 * @author cantunca
 */
public class SnakeGame implements KeyListener {

	private AnimationCanvas gameCanvas;
	private Snake snake;
	private Food food;
	private static final int FRAME_RATE = 50;
	private static final int SNAKE_SPEED = 10; // unit: grid square per second
	
	/**
	 * Constructor creating initial snake and food.
	 * It also starts the animation.
	 * @param gui GUI to draw on
	 */
	public SnakeGame(GUI gui) {
		
		gameCanvas = new AnimationCanvas(400, 400, 20, FRAME_RATE);
		gui.addCanvas(gameCanvas);
		
		// Add key listener
		gameCanvas.addKeyListener(this);
		gameCanvas.setFocusable(true);
		
		// Create initial snake
		snake = new Snake(this, FRAME_RATE/SNAKE_SPEED);
		gameCanvas.addObject(snake);
		
		// Create initial food
		food = new Food();
		replaceFood();
		gameCanvas.addObject(food);
		
		// Start animation
		gameCanvas.start();
	}

	/**
	 * Teleport the food to a random location
	 * that does not coincide with the snake.
	 */
	private void replaceFood() {
		int x, y;
		do {
			x = (int) (Math.random() * gameCanvas.getGridWidth());
			y = (int) (Math.random() * gameCanvas.getGridHeight());
		} while (snake.containsLocation(x, y));
		
		food.setX(x);
		food.setY(y);
	}
	
	/**
	 * Check if the food is in the given location.
	 * @param x X grid coordinate to be checked
	 * @param y Y grid coordinate to be checked
	 * @return true if food is in given location, false otherwise
	 */
	public boolean isFoodInLocation(int x, int y) {
		if (food.getX()==x && food.getY()==y) {
			return true;
		}
		return false;
	}
	
	/**
	 * Consume the food on the game board.
	 * Rather than deleting and creating a new food, it just replaces it.
	 */
	public void consumeFood() {
		replaceFood();
	}
	
	/**
	 * Restart game.
	 */
	private void restartGame() {
		gameCanvas.removeObject(snake);
		gameCanvas.removeObject(food);
		snake = new Snake(this, FRAME_RATE/SNAKE_SPEED);
		food = new Food();
		replaceFood();
		gameCanvas.addObject(snake);
		gameCanvas.addObject(food);
	}

	@Override
	public void keyPressed(KeyEvent e) {
		// Capture arrow keys for snake movement
		if (e.getKeyCode() == KeyEvent.VK_UP) {
			snake.setDirection(Direction.UP);
		} else if (e.getKeyCode() == KeyEvent.VK_DOWN) {
			snake.setDirection(Direction.DOWN);
		} else if (e.getKeyCode() == KeyEvent.VK_LEFT) {
			snake.setDirection(Direction.LEFT);
		} else if (e.getKeyCode() == KeyEvent.VK_RIGHT) {
			snake.setDirection(Direction.RIGHT);
		}
	}

	@Override
	public void keyTyped(KeyEvent e) {
		// Restart game command (R key)
		if (e.getKeyChar() == 'r') {
			restartGame();
		}
	}
	
	@Override
	public void keyReleased(KeyEvent e) {
		// Nothing to capture
	}
	
}
