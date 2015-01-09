#include "chrono_parallel/solver/ChSolverGD.h"

using namespace chrono;

uint ChSolverGD::SolveGD(const uint max_iter, const uint size, blaze::DynamicVector<real>& mb, blaze::DynamicVector<real>& ml) {
  real eps = step_size;
  r.resize(size);

  ShurProduct(ml, r);    // r = data_container->host_data.D_T *
                         // (data_container->host_data.M_invD * ml);
  r = mb - r;
  real resold = 1, resnew, normb = sqrt((mb, mb));
  if (normb == 0.0) {
    normb = 1;
  };
  for (current_iteration = 0; current_iteration < max_iter; current_iteration++) {
    ml = ml + eps * r;
    r = data_container->host_data.D_T * (data_container->host_data.M_invD * ml);
    r = mb - r;
    resnew = sqrt((ml, ml));
    residual = std::abs(resnew - resold);
    AtIterationEnd(residual, GetObjectiveBlaze(ml, mb), iter_hist.size());
    if (residual < data_container->settings.solver.tolerance) {
      break;
    }
    resold = resnew;
  }
  Project(ml.data());

  return current_iteration;
}
