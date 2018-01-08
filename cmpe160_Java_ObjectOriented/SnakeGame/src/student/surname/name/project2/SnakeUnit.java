package student.surname.name.project2;

/**
 * Represents a square of the snake body in Classic Snake Game.
 * @author cantunca
 */
public class SnakeUnit {
	
	private int x, y;
	
	public SnakeUnit(int x, int y) {
		this.x = x;
		this.y = y;
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
	
	/**
	 * Move in the given direction one grid square.
	 * @param dir Direction to move
	 */
	public void moveInDirection(Direction dir) {
		if (dir == Direction.UP) {
			y++;
		} else if (dir == Direction.DOWN) {
			y--;
		} else if (dir == Direction.LEFT) {
			x--;
		} else if (dir == Direction.RIGHT) {
			x++;
		}
	}
	
	/**
	 * Create a duplicate.
	 */
	public SnakeUnit clone() {
		return new SnakeUnit(x, y);
	}

}
