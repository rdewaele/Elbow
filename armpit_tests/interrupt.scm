(define (hz->us hz) (round (/ 1000000 hz)))

(begin
  (define timer0 #xE000400)  
  (define timer0-int 4)      
  (define timer-period #x18) 
  (define timer-count  #x08) 
  (define (stop timer)
    (write 0 timer #x04))    
  (define (restart timer)
    (write 2 timer #x04)     
    (write 1 timer #x04))    
  (write 59 timer0 #x0C)     
  (write 3  timer0 #x14)
  (stop timer0)
  (write 2272 timer0 timer-period)
  (write 500 timer0 timer-period)
)  ;;should be 440Hz


(begin
  (define ledport #xE002800)
  (define pin-status #x00)
  (define pin-set #x04)
  (define pin-dir #x08)
  (define pin-clear  #x0C)
  (define (set-pin port pin)
    (write (ash 1 (min pin 29)) port pin-set))
  (define (clear-pin port pin)
    (write (ash 1 (min pin 29)) port pin-clear))
  (define (is-set? port pin)
    (not (zero? (logand (read port pin-status) (ash 1 (min pin 29))))))
  (define (toggle port pin)
    (if (is-set? port pin)
	(clear-pin port pin)
	(set-pin port pin)))
  (write -1 ledport pin-dir) ;; set all to output
)

(write (lambda (int) 
	 (toggle ledport 16))
       timer0
       #x010000)
