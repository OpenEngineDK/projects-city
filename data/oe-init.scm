(define funs '())

(define add-to-process (lambda (fn)
	(set! funs (cons fn funs))
))

(define apply-process (lambda (n)
	(map (lambda (fn)
			(apply fn n '())
		
		) funs)))
	
	

(define process (lambda (n)
	(display n)
	(display "\n")
	(apply-process n)
	))

