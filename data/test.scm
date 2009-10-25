(define test 
		(lambda (n)
			;(vec-to-list (tn-get-pos dot-tn))
			;(display 'test)
			(let* (	(v (vec-to-list (oe-call dot-tn 'get-position)))
				  	(x (car v))
					(y (cadr v))
					(z (caddr v))
					(n-vec `(,x ,y ,(+ z 1))))
				(oe-call dot-tn 'set-position (list-to-vec n-vec))
			
				(display n-vec)
			)	
			))
			
(add-to-process test)
