
// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#include "Tests.h"

#include "Particles.h"
#include "DataStripes.h"
#include "Integrator.h"
#include "Emitter.h"
#include "Render.h"

ParticleSystemTestVelocity::ParticleSystemTestVelocity()
: particles(new Particles()) {
    particles->dataStripes().addStripe(DataStripe::kFloat32_3, "pos");
    particles->dataStripes().addStripe(DataStripe::kFloat32_3, "vel");
    std::shared_ptr<Integrator> integrate(new Integrator());
    std::shared_ptr<Emitter> emitter(new Emitter());
    std::shared_ptr<AccelerationField> acc(new AccelerationField());
    acc->setDirection(0, 0, 1);
    integrate->addField(acc);
    particles->addIntegrator(integrate);
    particles->addEmitter(emitter);
    particles->dataStripes().resize(2048);
}

void ParticleSystemTestVelocity::update(float t, float dt) {
    particles->update(t, dt);
}

void ParticleSystemTestVelocity::render(const float* mvp) {
    static Render* r = new Render(particles);
    r->render(mvp);
}
