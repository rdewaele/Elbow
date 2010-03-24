(begin
	(define timer0 #xE000400)  ; T0 (IR = #x00, TCR = #x04, TC = #x08, PR = #x0C, MCR = #x14, MR0 = #x18)
	(define timer0-int 4)      ; timer0 interrupt number
	(define timer-period #x18) ; TxMR0 offset
	(define timer-count  #x08) ; TxTC  offset
	(define (stop timer)
		(write 0 timer #x04))    ; disable timer via TxTCR
	(define (restart timer)
		(write 2 timer #x04)     ; reset timer via TxTCR
		(write 1 timer #x04))    ; enable and start timer via TxTCR
	(write 59 timer0 #x0C)     ; set timer0 period to 1 us (for 60MHz clk) via T0PR
	(write 3  timer0 #x14)     ; set timer0 to generate interrupt and stop on MR0 match via T0MCR
	(stop timer0)
	(write 1000000 timer0 timer-period))

(let ((keep-looping #t)
			(counter 0))

	(define (stop-game-loop) (set! keep-looping #f))

	(define (check-counter old-counter)
		(> counter old-counter))

	(define (increment-counter)
		(set! counter (+ counter 1)))

	(write (lambda (int)
					 (cond ((= int 4)
									(display "tik tak: ") (newline)
									(increment-counter))
								 (else (stop-game-loop)))
					 )
				 timer0
				 #x010000)

	(define (game-loop time-counter)
		(if (check-counter)
			(begin  (display "collect input/sensor info")       (newline)
							(display "evolving game state")             (newline)
							(display "drawing game state (if changed)") (newline)))
		(if keep-looping
			(game-loop)
			(display "Goodbye cruel world!")))
	(restart timer0)
	(game-loop)
	)
