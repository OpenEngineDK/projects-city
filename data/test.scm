(define dt 1)

(define test 
	(lambda (n)
		;(vec-to-list (tn-get-pos dot-tn))
		;(display 'test)
		(set! dt (+ dt (/ n 5000000)))
		(let* (	(v (vec-to-list (oe-call dot-tn 'get-position)))
			  	(x (car v))
				(y (cadr v))
				(z (caddr v))
					
			(n-vec `(,(+ (* (sin dt) 200) 400)
					,y 
					,(+ (* (cos dt) 200) 400))))
		(oe-call dot-tn 'set-position (list-to-vec n-vec))
		
		;(display dt)
		)))
	
			
(add-to-process 'test test)
