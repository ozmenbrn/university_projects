package visualization;

import java.awt.Color;
import java.awt.Font;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.ArrayList;

import javax.swing.JFrame;

import acm.graphics.GCanvas;
import acm.graphics.GCompound;
import acm.graphics.GImage;
import acm.graphics.GLabel;
import acm.graphics.GObject;

@SuppressWarnings("serial")
public class Board extends GCompound implements BoardIntf {
	private static final int TURTLE_WIDTH = 80;
	private static final String TURTLE_IMAGE_PATH = "turtle.png";
	private static final String BACKGROUND_IMAGE_PATH = "asfalt.jpg";

	public JFrame frame;
	private GCanvas canvas;
	private GImage turtle;
	private GImage background;
	private GLabel round;
	private GLabel score;
	private GLabel highScore;
	private GLabel gameover;
	
	private int roundnum = 1;
	private int scorenum = 0;

	private ArrayList<GObject> objects = new ArrayList<GObject>();
	
	public Board(String boardName, int width, int height) {
		setCanvas(boardName, width, height);
		setBackground();
		addTurtle();
		addGameInfoLabels();
	}

	@Override
	public void setCanvas(String boardName, int width, int height) {
		frame = new JFrame(boardName);
		frame.setSize(width, height);
		canvas = new GCanvas();
		frame.getContentPane().add(canvas);

		frame.setVisible(true);
		frame.addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent windowEvent) {
				System.exit(0);
			}
		});
		addKeyBoardListener();
		
	}

	@Override
	public void setBackground() {
		background = new GImage(BACKGROUND_IMAGE_PATH);
		canvas.add(background);
		background.sendBackward();
		
	}

	@Override
	public void addKeyBoardListener() {
		frame.addKeyListener(new KeyListener() {

			@Override
			public void keyTyped(KeyEvent e) {
			}

			@Override
			public void keyReleased(KeyEvent e) {
			}

			@Override
			public void keyPressed(KeyEvent event) {
				int keyCode = event.getKeyCode();
				if (keyCode == KeyEvent.VK_UP) {
					turtle.move(0, -8);
				} else if (keyCode == KeyEvent.VK_DOWN) {
					turtle.move(0, 8);
				} else if (keyCode == KeyEvent.VK_RIGHT) {
					turtle.move(8, 0);
				} else if (keyCode == KeyEvent.VK_LEFT) {
					turtle.move(-8, 0);
				}
			}
		});
		
	}
	
	
	public void addTurtle() {
		turtle = new GImage(TURTLE_IMAGE_PATH);
		turtle.setSize(TURTLE_WIDTH,TURTLE_WIDTH);
		// turtle = new GTurtle();
		// turtle.setDirection(90);
		turtle.setLocation(frame.getWidth() / 2, frame.getHeight()- TURTLE_WIDTH*5/3);
		canvas.add(turtle);
		System.out.println("added");
		turtle.sendToFront();
	}

	
	@Override
	public void addObject(GObject g) {
		canvas.add(g);
		objects.add(g);
		turtle.sendToFront();
	}
	
	@Override
	public void remove(GObject g) {
		canvas.remove(g);
		objects.remove(g);
		turtle.sendToFront();
	}
	

	@Override
	public void waitFor(long millisecs) {
		try {
			Thread.sleep(millisecs);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	
	@Override
	public void addGameInfoLabels() {
		round = new GLabel("Round: " + roundnum);
		score = new GLabel("Score: " + scorenum);
		round.setFont(new Font("Arial", Font.BOLD, 25));
		round.setColor(Color.BLACK);
		score.setFont(new Font("Arial", Font.BOLD, 25));
		score.setColor(Color.BLACK);
		add(round, 70, 900);
		add(score, 70, 930);
		canvas.add(score);
		canvas.add(round);
		score.sendToFront();
		round.sendToFront();
	}
	
	public int getRoundnum() {
		return roundnum;
	}

	public void setRoundnum(int roundnum) {
		this.roundnum = roundnum;
	}

	public int getScorenum() {
		return scorenum;
	}

	public void setScorenum(int scorenum) {
		this.scorenum = scorenum;
	}
	
	public double getTurtleY(){
		return turtle.getY();
	}
	
	public double getTurtleX(){
		return turtle.getX();
	}
	
	public void removeTurtle(){
		canvas.remove(turtle);
		objects.remove(turtle);
	}
	
	public void removeGameInfoLabels(){
		canvas.remove(score);
		canvas.remove(round);
	}
	
	public void gameOver(){
		canvas.removeAll();
		highScore = new GLabel("Score: " + scorenum);
		highScore.setFont(new Font("Arial", Font.BOLD, 50));
		highScore.setColor(Color.BLACK);
		add(highScore, 500, 550);
		gameover = new GLabel("GAME OVER");
		gameover.setFont(new Font("Arial", Font.BOLD, 50));
		gameover.setColor(Color.BLACK);
		add(gameover, 500, 450);
		canvas.add(highScore);
		canvas.add(gameover);
	}

}
