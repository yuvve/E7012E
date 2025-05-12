#include "slidingMode.h"
#include "state.h"
#include "constants.h"

void setupSlidingMode(slidingMode *self, float T, float epsilon, float lambda) {
  self->T = T;
  self->epsilon = epsilon;
  self->lambda = lambda;
  self->p[0] = 0.0f;
  self->p[1] = 0.0f;
  self->sigma = 0.0f;
}

void setupState(state *self) {
  self->theta = 0.0f;
  self->error = 0.0f;
  self->alpha = 0.0f;
  self->speed = 0.0f;
  self->hjulbas = CAR_HJULBAS;
  self->bredd = CAR_SENSORBREDD;
  self->korridorBredd = KORRIDOR_BREDD;
}

// uppdaterar tillstånden error och theta med hjälp av proximity mätningar
void measurement(state *self, float d, float b) {
  self->theta = acos(self->korridorBredd/(b+d+self->bredd));
  self->error = (b-d)*cos(self->theta);
}

// värdet på sliding surface för givet tillstånd
void calcSigma(state *self, slidingMode *controller) {
  controller->sigma = controller->p[0]*self->error + controller->p[1]*self->error;
}

// överföringsfunktion x_k+1 = f(x_k,u_k)
void stateDynamics(state *self, slidingMode *controller, float f[2]){
  f[0] = self->theta + controller->T/self->hjulbas*tan(self->alpha)*self->speed;
  f[1] = cos(self->theta)*(self->error - 2*controller->T*self->speed*sin(self->theta));
}

// objektfunktion för sliding mode
float objektFunktion(state *self, slidingMode *controller) {
  float f[2];
  stateDynamics(self,controller,f);
  return controller->p[0]*f[0] + controller->p[1]*f[1] - (1-controller->lambda)*(controller->p[0]*self->error + controller->p[1]*self->theta);
}

// gradienten av objektfunktionen
float gradF(state *self, slidingMode *controller) {
  return controller->p[0]*controller->T/self->hjulbas/pow(cos(self->alpha),2);
}

// sliding mode control med Newton Raphson med addaptiv steglängd, säker konvergens
void slidingModeControl(state *self, slidingMode *controller) {
  calcSigma(self, controller);

  float c = 0.1;
  float h = 1.0;
  float F = objektFunktion(self, controller);
  float dF = gradF(self, controller);

  while (abs(F) > controller->epsilon) {
    float alpha_old = self->alpha;
    float new_alpha = alpha_old - h * F / dF;

    // Update alpha temporärt för Wolfe vilkoren
    self->alpha = new_alpha;
    float F_new = objektFunktion(self, controller);

    while (F_new > (1 - c*h)*F) {
      h = h / 2;
      new_alpha = alpha_old - h * F / dF;
      self->alpha = new_alpha;
      F_new = objektFunktion(self, controller);
    }

    F = F_new;
    dF = gradF(self, controller);
  }
}

// räknar ut tiden för en sväng samtidigt som den sätter vinkeln på hjulen genom att beräkna svängradien med felet från centerlinjen och korridorbredden
// här antas bilen köra rakt, bör gå att ordan med hjälp av bilens vinkel från centerlinjen
float svang(state *self) {
  float r = (self->korridorBredd/2 - self->error - self->bredd/2); // svängradien
  self->alpha = atan(self->hjulbas/r);
  return 3.1415/2*r/pow(self->speed,2); // tiden för en sväng i konstant hastighet
}







