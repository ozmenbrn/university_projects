all: ADVCALC.jar

ADVCALC.jar: ADVCALC.class
	@jar cvfe ADVCALC.jar ADVCALC ADVCALC.class

ADVCALC.class: ADVCALC.java
	@javac ADVCALC.java
	
run: ADVCALC.jar
	@java -jar ADVCALC.jar ${ARGS}
	
clean:
	@rm ADVCALC.jar *.class
