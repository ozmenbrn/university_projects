(defglobal ?*inputs* = (create$)) 		;inputs.txt file is saved in that global variable
(defglobal ?*symptoms* = (create$)) 		;symptoms file is saved in that global variable
(defglobal ?*illness* = (create$)) 		;illness.txt file is saved in that global variable
(defglobal ?*diagnoses* = (create$))		;final diagnoses of patients that will be on output file
(defglobal ?*answers* = (create$))		;get answers of each patients into that variable
(defglobal ?*tempList* = (create$))		;temporaray list
(defglobal ?*inputIds* = (create$))		;input ids in case of ids are not in ascending order

;FUNCTION for reading all lines and store it in corresponding global variable decleared on top
(deffunction readAll(?input1 ?input2 ?input3)  	
	(reset)
	(bind ?data (readline ?input1)) 		;bind the the first line to the variable called ?data
	(while (neq ?data EOF)				;read until its end
		(if (neq ?data "")				;readAll method prints "" at all end lines. If statement is for to get rid of it
			then (bind ?*inputs* (insert$ (create$ ?data) 1 ?*inputs*)))          ;add the line to the global variable inputs.
    		(bind ?data (readline ?input1)))	 		;take the next input
    	(close ?input1)							;close file when reading is finished
    	
    	(bind ?data (readline ?input2))		;rest is same as first while loop.
	(while (neq ?data EOF)
		(if (neq ?data "")
			then (bind ?*symptoms* (insert$ (create$ ?data) 1 ?*symptoms*))) 
    		(bind ?data (readline ?input2)))	
    	(close ?input2)
    	
    	(bind ?data (readline ?input3))		;same loop again
	(while (neq ?data EOF)
		(if (neq ?data "")
			then (bind ?*illness* (insert$ (create$ ?data) 1 ?*illness*))) 
    		(bind ?data (readline ?input3)))	
    	(close ?input3))
	

;FUNCTION for opening and reading input file
(deffunction readFile ()
	(open "INPUTS.txt" inputsFile "r")
	(open "SYMPTOMS.txt" symptomsFile "r")
	(open "ILLNESS.txt" illnessFile "r")
	(readAll inputsFile symptomsFile illnessFile))	     ;Call function to read and stor the lines into global variables.
	

;FUNCTION for splitting line from "," symbol.
(deffunction splitInputs(?line)
	(bind ?line (str-cat ?line ",dummy,dummy"))
	(bind ?startIndex (+ (str-index "," ?line) 1))
	(bind ?*inputIds* (insert$ (create$ (sub-string 1 (- ?startIndex 2) ?line)) 1 ?*inputIds*)) 
	(bind ?restString (sub-string ?startIndex (str-length ?line) ?line))
	(bind ?endIndex (+ -1 (str-index "," ?restString) ?startIndex))
	(while (< (+ (str-index "," ?restString) ?endIndex) (str-length ?line))
		(bind ?*answers* (insert$ (create$ (sub-string ?startIndex (- ?endIndex 1) ?line)) 1 ?*answers*)) 
		(bind ?startIndex (+ ?endIndex 1)) 
		(bind ?restString (sub-string ?startIndex (str-length ?line) ?line))
		(bind ?endIndex (+ (str-index "," ?restString) ?endIndex))
	)
)
		
;FUNCTION for spliting illnesses from "," symbol.
(deffunction splitIllness(?line)
	(bind ?line (str-cat ?line ",dummy,dummy"))
	(bind ?startIndex 1)
	(bind ?restString (sub-string ?startIndex (str-length ?line) ?line))
	(bind ?endIndex (+ -1 (str-index "," ?restString) ?startIndex))
	(while (neq (sub-string ?startIndex (- ?endIndex 1) ?line) "999")
		(bind ?*tempList* (insert$ (create$ (sub-string ?startIndex (- ?endIndex 1) ?line)) 1 ?*tempList*)) 
		(bind ?startIndex (+ ?endIndex 1)) 
		(bind ?restString (sub-string ?startIndex (str-length ?line) ?line))
		(bind ?endIndex (+ (str-index "," ?restString) ?endIndex))
	)
)
	
;Converts string to an integer
(defmethod integer ((?s STRING))
   (integer (string-to-field ?s)))
   
;Converts string to an float
(defmethod float ((?s STRING))
   (float (string-to-field ?s)))	

;MAIN ALGORITHM
;FUNCTION that calculates bayes scores for choosing illness and assign it to diagnoses variable
(deffunction assignIllness()
	(loop-for-count (?personNum 1 (length$ ?*inputs*)) do     ;For every patient(people) it loops
		(bind ?*answers* (create$))			;Creates empty answer list to be filled with each person's answers'
		(bind ?maxScore 0)                    ;for every user we need to assign maxScore to 0 due to changed answers
		(bind ?selectedIllness "none")        ;Before calculating scores, selected ilnesss is set to "none"
		(splitInputs (nth$ ?personNum ?*inputs*))         ;This function splits each answer from "," symbol to get "yes" or "no" answers
		(loop-for-count (?illnessNum 1 (length$ ?*illness*)) do     ;for every illnesses it loops
			(bind ?*tempList* (create$))                            ;Every time it loops, assign tempList to empty List
			(splitIllness (nth$ ?illnessNum ?*illness*))            ;Function called which split neccessary information from "," symbol
			(bind ?knownScore 0.0000001)              ;Prior Probabilty at 2nd element and assign it to score to multiply next
			(if (neq (float (nth$ 2 ?*tempList*)) 0.0) then (bind ?knownScore (log (float (nth$ 2 ?*tempList*)))))
			(bind ?knownNotScore 0.0000001)     ;1 - Prior Probabilty at 2nd element and assign it to score to multiply next					
			(if (neq (- 1 (float (nth$ 2 ?*tempList*))) 0.0) then (bind ?knownNotScore (log (- 1 (float (nth$ 2 ?*tempList*))))))
			(bind ?ans -1)                    	                  ;it will be "yes" or "no" in the following loops
			(loop-for-count (?i 3 (length$ ?*tempList*)) do         ;for each element in a single line of illnesses
				(if (eq (mod ?i 3) 0) then	                      ;(Symptom number),That means we are in the index
					(bind ?ans (nth$ (integer (nth$ ?i ?*tempList*)) ?*answers*)))    ;get the answer from the index of inputs list 
				(if (eq (mod ?i 3) 1) then	                      ;P(E|H) value
					(if (eq ?ans "yes") then
						(bind ?probability 0.0000001)
						(if (neq (float (nth$ ?i ?*tempList*)) 0.0) then
							(bind ?probability (log (float (nth$ ?i ?*tempList*)))))                                
						(bind ?knownScore (+ ?knownScore ?probability)))         ;Multiply score with the value of P(E|H)
					(if (eq ?ans "no") then
						(bind ?probability 0.0000001)
						(if (neq (- 1 (float (nth$ ?i ?*tempList*))) 0.0) then
							(bind ?probability (log (- 1 (float (nth$ ?i ?*tempList*))))))
						(bind ?knownScore (+ ?knownScore ?probability))))   ;;Multiply score with the value of P(~E|H)
				(if (eq (mod ?i 3) 2) then	                      ;P(E|~H) value
					(if (eq ?ans "yes") then
						(bind ?probability 0.0000001)
						(if (neq (float (nth$ ?i ?*tempList*)) 0.0) then
							(bind ?probability (log (float (nth$ ?i ?*tempList*)))))                                
						(bind ?knownNotScore (+ ?knownNotScore ?probability)))         ;Multiply score with the value of P(E|~H)
					(if (eq ?ans "no") then
						(bind ?probability 0.0000001)
						(if (neq (- 1 (float (nth$ ?i ?*tempList*))) 0.0) then
							(bind ?probability (log (- 1 (float (nth$ ?i ?*tempList*))))))
						(bind ?knownNotScore (+ ?knownNotScore ?probability)))   ;;Multiply score with the value of P(~E|~H)

				) 
			)
			(bind ?knownScore (exp ?knownScore))
			(bind ?knownNotScore (exp ?knownNotScore))	
			(bind ?score (/ ?knownScore (+ ?knownScore ?knownNotScore)))
			(if (< ?maxScore ?score) then                           ;update the maxScores and illness name according to maxScore
				(bind ?maxScore ?score)
				(bind ?selectedIllness (nth$ 1 ?*tempList*)))
		)
		(bind ?*diagnoses* (insert$ (create$ ?selectedIllness) 1 ?*diagnoses*))      ;Push selected illness to diagnoses list
    )
)

;FUNCTION to write file named "OUTPUTS.txt"
(deffunction writeFile()
	(open "OUTPUTS.txt" outputFile "w")
	(loop-for-count (?j 1 (length$ ?*inputIds*)) do
        	(printout outputFile (nth$ ?j ?*inputIds*))
        	(printout outputFile ",")
        	(printout outputFile (nth$ ?j ?*diagnoses*))
        	(if (neq ?j (length$ ?*inputs*)) then
        		(printout outputFile "" crlf)
        		(printout outputFile "" crlf)
        	)
	)
	(close outputFile)
)

;MAIN FUNCTION
(deffunction runCode()
	(printout t "Reading files..." crlf)
	(readFile)
	(printout t "Creating dictionary for illnesses and assigning them to each user..." crlf)
	(assignIllness)
	(printout t "Creating file named OUTPUTS.txt..." crlf)
	(writeFile)
	(printout t "Done!" crlf))


