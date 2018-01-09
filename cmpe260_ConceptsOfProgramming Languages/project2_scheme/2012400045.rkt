#lang scheme
; compiling: yes
; complete: yes
; 2012400045

;(4.1 card-color one-card) (or a b) if the selected card is H or D returns red else black. 
(define (card-color card)             
  (if (or (equal? (car card) '|H|) (equal? (car card) '|D|)) '|red| '|black|)
)

;(4.2 card-rank one-card) (4 conditions for each A K Q J values and rest of them return the value itself)
(define (card-rank card1)
  (cond
    ((equal? (cdr card1) '|A|) 11)
    ((equal? (cdr card1) '|K|) 10) ((equal? (cdr card1) '|Q|) 10) ((equal? (cdr card1) '|J|) 10)
    (else (cdr card1))
  )
)

(define (f h)
  (cond
    ((null? h) 1)
    (else 2)
    )
  )

(define hello (lambda (x y z) (/ x y z)))

;(4.3 all-same-color list-of-cards)
(define (all-same-color list-of-cards)
  (cond
    ((null?  list-of-cards) "Empty List")    ;check if it list-of-cards empty or not
    ((eq? (length list-of-cards) 1) #t)      ;if the player has 1 card only in his hand then return true
    ((equal? (card-color (car list-of-cards)) (card-color (car (cdr list-of-cards)))) (all-same-color (cdr list-of-cards))) ;check if the 2 pair in list have same color. 1st and 2nd element every time
    (else     ;if two next cards have different color than return false
         #f))
)

;(4.4 fdraw list-of-cards held-cards)
(define (fdraw list-of-cards held-cards)
  (append held-cards (cons (car list-of-cards) '()))      ;append function to add card from list-of-cards to held cards
)

;4.5 (fdiscard list-of-cards list-of-moves goal held-cards)
(define (fdiscard list-of-cards list-of-moves goal held-cards)  
  (cdr held-cards)                                    ;basiccally return the rest of card lists. No strategy just play simple with a one strict rule. 
)



;(Function for 4.6) Helps find-steps to keep lists such as Own-List and Own-Held
(define (own-cards list-of-cards list-of-moves goal own-list own-held)    
  (cond
    ((null? list-of-moves) own-list)      ;"Game-Over 1" Condition                  
    ((< goal (calc-playerpoint (find-held-cards own-list))) own-list) ;"Game-Over 4" Condition 
    ((and (equal? (car list-of-moves) '|draw|) (null? list-of-cards)) own-list) ;"Game-Over 2" Condition
    ((and (equal? (car list-of-moves) '|discard|) (null? own-held)) own-list) ;"Game-Over 3" Condition
    (else                                  ;While game goes still...
     (if (equal? (car list-of-moves) '|draw|) (own-cards (cdr list-of-cards) (cdr list-of-moves) goal (append own-list (cons (list '|draw| (car list-of-cards)) '())) (append own-held (cons (car list-of-cards) '())))
         ;if next move is DRAW, recurcively call own-cards function with rest of card-list and move, Goal, Own-list which will return when game over (add draw to first element of card list and make it list) as own-list
         ;last Own-held helps to keep held card list to be use.
         (own-cards list-of-cards (cdr list-of-moves) goal (append own-list (cons (list '|discard| (car own-held)) '())) (remove (car own-held) own-held)))
     )   ;if next move is DISCARD send rest of list moves and goal and append discard with first element of held cards to a list and remove that card from held cards.
  )
)
;4.6 (find-steps list-of-cards list-of-moves goal)
(define (find-steps list-of-cards list-of-moves goal)
  (own-cards list-of-cards list-of-moves goal '() '())
)


;(Function for 4.7) Helps find-held-cards to keep lists such as Held-list
(define (held-cards list-of-steps held-list)
  (cond
    ((null? list-of-steps) held-list)  ;if move list is empty than return
    ((equal? (car (car list-of-steps)) '|draw|) (held-cards (cdr list-of-steps) (append held-list (cdr (car list-of-steps))))) ;if equal draw, append it to the held-list
    (else (held-cards (cdr list-of-steps) (remove (car (cdr (car list-of-steps))) held-list)) ;else remove it from held-list
  ))
)
;4.7 (find-held-cards list-of-steps)
(define (find-held-cards list-of-steps)
  (held-cards list-of-steps '())
)


;4.8 (calc-playerpoint list-of-cards)
(define (calc-playerpoint list-of-cards)
  (cond
    ((null? list-of-cards) 0)  ;if the list is empty return 0 recursively
      (else
        (+ (card-rank (car list-of-cards)) (calc-playerpoint (cdr list-of-cards))) ;recursive function which add numbers from 0 + ... +last rank
      )
  )
)


;4.9 (calc-score list-of-cards goal)
(define (calc-score list-of-cards goal)
  (cond         ;Condition if goal is not exceeded, than do these math functions according to the project description and Make INTIGER DIVISION with "floor"
    ((> goal (calc-playerpoint list-of-cards)) (if (all-same-color list-of-cards) (floor (/ ( - goal (calc-playerpoint list-of-cards)) 2 )) (floor ( - goal (calc-playerpoint list-of-cards)))))
    ((if (all-same-color list-of-cards) (floor (/ (* 5 ( - (calc-playerpoint list-of-cards) goal)) 2 )) (floor (* 5 ( - (calc-playerpoint list-of-cards) goal)))))
  )         ;Else means goal is exceeded and there must be penalties. 
)


;4.10 (play list-of-cards list-of-moves goal)
(define (play list-of-cards list-of-moves goal)  ;Basically use function from above and play it.
  (calc-score (find-held-cards (find-steps list-of-cards list-of-moves goal)) goal)
)


