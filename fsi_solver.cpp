#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

const int N = 200;              // Number of grid points
const double L = 1.0;           // Length of the pipe
const double dx = L / (N - 1);  // Grid spacing
const double dt = 0.002;       // Smaller time step
const double rho_f = 1.0;       // Fluid density
const double rho_s = 2.0;       // Structure density
const double nu = 0.005;         // Fluid viscosity
const double E = 5.0;           // Young's modulus of the structure
const int steps = 1500;         // Number of time steps

std::vector<double> u_f(N, 0.0);  // Fluid velocity
std::vector<double> p(N, 0.0);    // Fluid pressure
std::vector<double> u_s(N, 0.0);  // Structure displacement

// Function to update the fluid equations
void updateFluid() {
    for (int i = 1; i < N - 1; i++) {
        u_f[i] += dt * (
            -u_f[i] * (u_f[i] - u_f[i - 1]) / dx  // Convection
            - (p[i + 1] - p[i - 1]) / (2 * dx * rho_f)  // Pressure gradient
            + nu * (u_f[i + 1] - 2 * u_f[i] + u_f[i - 1]) / (dx * dx)  // Diffusion
        );
    }
}

void updateStructure() {
   for (int i = 1; i < N - 1; i++) {
        u_s[i] += dt * dt * (
            E * (u_s[i + 1] - 2 * u_s[i] + u_s[i - 1]) / (dx * dx * rho_s)  // Elastic force
            + p[i] / rho_s  // Fluid pressure force
        );
    }
}

void enforceCoupling() {
    for (int i = 1; i < N - 1; i++) {
        p[i] = E * (u_s[i + 1] - 2 * u_s[i] + u_s[i - 1]) / (dx * dx);  // Pressure from structure displacement
    }
}

int main() {
    for (int i = 0; i < N; i++) {
        u_f[i] = 0.1 * sin(2 * M_PI * i / N);
    }
	
    for (int step = 0; step < steps; step++) {
        updateFluid();
        updateStructure();
        enforceCoupling();

        if (step % 100 == 0) {
            std::cout << "Step " << step << ": u_s[mid] = " << u_s[N / 2]
                      << ", u_f[mid] = " << u_f[N / 2]
                      << ", p[mid] = " << p[N / 2] << std::endl;
        }
    }
	
	std::ofstream file("fsi_data.txt");

for (int step = 0; step < steps; step++) {
    updateFluid();
    updateStructure();
    enforceCoupling();

    if (step % 10 == 0) {
        for (int i = 0; i < N; i++) {
            file << u_s[i] << " " << u_f[i] << "\n";
        }
        file << "\n"; 
    }
}

file.close();
    return 0;
}