#include <GL/glut.h>
#include <Eigen/Dense>
#include <vector>

using namespace std;
using namespace Eigen;


// rendering hyperparameters
const static int WINDOW_WIDTH = 600;
const static int WINDOW_HEIGHT = 800;
const static float particleSizeRatio = 0.75; // size with respect to kernel radius

// initialization hyperparameters
const static float x_start_position = 10;
const static float y_start_position = 10;
const static int num_particle_col = 15;
const static int num_particle_row = 75;

// physics hyperparameters
const static float mass = 10; // the mass of each particle
const static float h = 10; // kernel radius
const static float K = 2000; // coefficient depend on the nature of local resistance (for computing pressure)
const static float rho0 = 1500; // rest density (for computing pressure)
const static float viscosity = 500; // viscosity (for computing viscosity part of acceleration)
const static float dt = 0.001; // timestep
const static float bounceRate = 0.1; // the bounce rate of a particle hitting a boundary
const static float gravityScale = 2000;

// constants
const static float M_PI = 3.14159265358979323846;
const static float boundWidth = h / 2;
const static Vector2d g(0, - gravityScale * 9.8); // acceleration due to gravity

// particle structure
class Particle {
public:
	Particle(float x, float y);
	Vector2d q, v, a; // coordinates, velocity, acceleration
	float rho, p; // density, pressure
};
Particle::Particle(float x, float y) {
	q << x, y;
	v << 0, 0;
	a << 0, 0;
	rho = 0;
	p = 0;
}
static vector<Particle> particles;

// step 1
void InitParticles(void) {
	for (unsigned int x_count = 0; x_count < num_particle_col; x_count++) {
		for (unsigned int y_count = 0; y_count < num_particle_row; y_count++) {
			float x = x_start_position + x_count * 0.9 * h;
			float y = y_start_position + y_count * h;

			// add randomness to x coordinates to make the simulation more interesting
			x += (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.2)) - 0.1) * h;
			particles.push_back(Particle(x, y));
		}
	}
}

// step 2
const static float densityConst = mass * 4 / (M_PI * pow(h, 8));
void UpdateDensityPressure(void) {
	for (auto& particle : particles) {
		particle.rho = 0;
		for (auto& particle_tmp : particles) {
			float diffNorm = (particle.q - particle_tmp.q).norm();
			if (diffNorm < h) {
				particle.rho += pow(pow(h, 2) - pow(diffNorm, 2), 3);
			}
		}
		particle.rho *= densityConst;
		particle.p = K * (particle.rho - rho0);
	}
}

// step 3
const static float pressureConst = - mass * 30 / (M_PI * pow(h, 5));
const static float viscosityConst = mass * viscosity * 20 / (3 * M_PI * pow(h, 5));
void UpdateAcceleration(void) {
	for (auto& particle : particles) {
		Vector2d a_press(0, 0);
		Vector2d a_visco(0, 0);
		for (auto& particle_tmp : particles) {
			Vector2d diff = particle.q - particle_tmp.q;
			if (diff.norm() < h) {
				a_press += (particle.p + particle_tmp.p) / (2 * particle.rho * particle_tmp.rho)
					* pow(h - diff.norm(), 2)
					* diff.normalized();
				a_visco += (particle_tmp.v - particle.v) / (particle.rho * particle_tmp.rho) * (h - diff.norm());
			}
		}
		particle.a = g + pressureConst * a_press + viscosityConst * a_visco;
	}
}

// step 4
void UpdateVelocityCoordinates(void) {
	for (auto& particle : particles) {
		particle.v += dt * particle.a;
		particle.q += dt * particle.v;
	}
}

// step 5
void BoundParticles(void) {
	for (auto& particle : particles) {
		if (particle.q(0) < boundWidth) {
			particle.q(0) = boundWidth;
			particle.v(0) *= -bounceRate;
		}
		if (particle.q(0) > WINDOW_WIDTH - boundWidth) {
			particle.q(0) = WINDOW_WIDTH - boundWidth;
			particle.v(0) *= -bounceRate;
		}
		if (particle.q(1) < boundWidth) {
			particle.q(1) = boundWidth;
			particle.v(1) *= -bounceRate;
		}
		if (particle.q(1) > WINDOW_HEIGHT - boundWidth) {
			particle.q(1) = WINDOW_HEIGHT - boundWidth;
			particle.v(1) *= -bounceRate;
		}
	}
}

void Update(void) {
	UpdateDensityPressure();
	UpdateAcceleration();
	UpdateVelocityCoordinates();
	BoundParticles();

	glutPostRedisplay();
}

void Render(void) {
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, 0, 1);

	// display particles
	glBegin(GL_POINTS);
	for (auto& particle : particles)
		glVertex2f(particle.q(0), particle.q(1));
	glEnd();

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 50);
	glutCreateWindow("SPH 2D Fluid Simulation");
	glClearColor(1.0, 1.0, 1.0, 1.0); // white background
	glColor4f(0.0, 0.4, 0.8, 1.0); // particle colour
	glEnable(GL_POINT_SMOOTH);
	glPointSize(particleSizeRatio * h);
	
	InitParticles();
	glutDisplayFunc(Render);
	glutIdleFunc(Update);
	glutMainLoop();

	return 0;
}