#include "stdint.h"

typedef struct {
    const int16_t b0;
    const int16_t b1;
    const int16_t b2;
    const int16_t a0;  
    const int16_t a1;
    const int16_t a2;
    const int16_t scale_factor;
    int16_t m_x1;
    int16_t m_x2;
    int16_t m_y1;
    int16_t m_y2;
}iir_t;

void iir_init(iir_t *iir){
    iir->m_x1 = 0;
    iir->m_x2 = 0;
    iir->m_y1 = 0;
    iir->m_y2 = 0;
}

static inline int iir_f(iir_t *iir, const int in)
{
	// calculate the output
	register int out_upscaled = (int)iir->b0*(int)in
		+ (int)iir->b1*(int)iir->m_x1 
		+ (int)iir->b2*(int)iir->m_x2
		- (int)iir->a1*(int)iir->m_y1 
		- (int)iir->a2*(int)iir->m_y2;
	// scale it back from int to short int
	short int out = out_upscaled >> iir->scale_factor;
	// update the delay lines
	iir->m_x2 = iir->m_x1;
	iir->m_y2 = iir->m_y1;
	iir->m_x1 = in;
	iir->m_y1 = out;
	return out;
}