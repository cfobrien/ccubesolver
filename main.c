#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#define PI 3.141592654
#define EPSILON 0.001
#define INPUTLEN 16

int heuristic;
double rotationMatrix[4][4];

typedef struct {
	double x;
	double y;
	double z;
} Vector;

const Vector r = { 1.0, 0.0, 0.0 };
const Vector w = { 0.0, 1.0, 0.0 };
const Vector b = { 0.0, 0.0, 1.0 };
const Vector o = { -1.0, 0.0, 0.0 };
const Vector y = { 0.0, -1.0, 0.0 };
const Vector g = { 0.0, 0.0, -1.0 };

typedef struct {
	Vector pos;
	Vector dir;
	char col;
} CubieFace;

CubieFace cube[54] = {
	//lower corners
	{ { -1.0, -1.0, -1.0 }, g, 'g' },
	{ { -1.0, -1.0, -1.0 }, y, 'y' },
	{ { -1.0, -1.0, -1.0 }, o, 'o' },

	{ { 1.0, -1.0, -1.0 }, g, 'g' },
	{ { 1.0, -1.0, -1.0 }, y, 'y' },
	{ { 1.0, -1.0, -1.0 }, r, 'r' },

	{ { -1.0, -1.0, 1.0 }, b, 'b' },
	{ { -1.0, -1.0, 1.0 }, y, 'y' },
	{ { -1.0, -1.0, 1.0 }, o, 'o' },

	{ { 1.0, -1.0, 1.0 }, r, 'r' },
	{ { 1.0, -1.0, 1.0 }, y, 'y' },
	{ { 1.0, -1.0, 1.0 }, b, 'b' },

	//upper corners
	{ { -1.0, 1.0, -1.0 }, g, 'g' },
	{ { -1.0, 1.0, -1.0 }, w, 'w' },
	{ { -1.0, 1.0, -1.0 }, o, 'o' },

	{ { 1.0, 1.0, -1.0 }, g, 'g' },
	{ { 1.0, 1.0, -1.0 }, w, 'w' },
	{ { 1.0, 1.0, -1.0 }, r, 'r' },

	{ { -1.0, 1.0, 1.0 }, b, 'b' },
	{ { -1.0, 1.0, 1.0 }, w, 'w' },
	{ { -1.0, 1.0, 1.0 }, o, 'o' },

	{ { 1.0, 1.0, 1.0 }, r, 'r' },
	{ { 1.0, 1.0, 1.0 }, w, 'w' },
	{ { 1.0, 1.0, 1.0 }, b, 'b' },

	//lower edges
	{ { -1.0, -1.0, 0.0 }, y, 'y' },
	{ { -1.0, -1.0, 0.0 }, o, 'o' },

	{ { 0.0, -1.0, -1.0 }, y, 'y' },
	{ { 0.0, -1.0, -1.0 }, g, 'g' },

	{ { 1.0, -1.0, 0.0 }, y, 'y' },
	{ { 1.0, -1.0, 0.0 }, r, 'r' },

	{ { 0.0, -1.0, 1.0 }, y, 'y' },
	{ { 0.0, -1.0, 1.0 }, b, 'b' },

	//upper edges
	{ { -1.0, 1.0, 0.0 }, w, 'w' },
	{ { -1.0, 1.0, 0.0 }, o, 'o' },

	{ { 0.0, 1.0, -1.0 }, w, 'w' },
	{ { 0.0, 1.0, -1.0 }, g, 'g' },

	{ { 1.0, 1.0, 0.0 }, w, 'w' },
	{ { 1.0, 1.0, 0.0 }, r, 'r' },

	{ { 0.0, 1.0, 1.0 }, w, 'w' },
	{ { 0.0, 1.0, 1.0 }, b, 'b' },

	//middle edges
	{ { -1.0, 0.0, -1.0 }, g, 'g' },
	{ { -1.0, 0.0, -1.0 }, o, 'o' },

	{ { 1.0, 0.0, -1.0 }, r, 'r' },
	{ { 1.0, 0.0, -1.0 }, g, 'g' },

	{ { -1.0, 0.0, 1.0 }, o, 'o' },
	{ { -1.0, 0.0, 1.0 }, b, 'b' },

	{ { 1.0, 0.0, 1.0 }, r, 'r' },
	{ { 1.0, 0.0, 1.0 }, b, 'b' },

	//centers
	{ r, r, 'r' },
	{ w, w, 'w' },
	{ b, b, 'b' },
	{ o, o, 'o' },
	{ y, y, 'y' },
	{ g, g, 'g' },
};

CubieFace reference[54] = {
	//lower corners
	{ { -1.0, -1.0, -1.0 }, g, 'g' },
	{ { -1.0, -1.0, -1.0 }, y, 'y' },
	{ { -1.0, -1.0, -1.0 }, o, 'o' },

	{ { 1.0, -1.0, -1.0 }, g, 'g' },
	{ { 1.0, -1.0, -1.0 }, y, 'y' },
	{ { 1.0, -1.0, -1.0 }, r, 'r' },

	{ { -1.0, -1.0, 1.0 }, b, 'b' },
	{ { -1.0, -1.0, 1.0 }, y, 'y' },
	{ { -1.0, -1.0, 1.0 }, o, 'o' },

	{ { 1.0, -1.0, 1.0 }, r, 'r' },
	{ { 1.0, -1.0, 1.0 }, y, 'y' },
	{ { 1.0, -1.0, 1.0 }, b, 'b' },

	//upper corners
	{ { -1.0, 1.0, -1.0 }, g, 'g' },
	{ { -1.0, 1.0, -1.0 }, w, 'w' },
	{ { -1.0, 1.0, -1.0 }, o, 'o' },

	{ { 1.0, 1.0, -1.0 }, g, 'g' },
	{ { 1.0, 1.0, -1.0 }, w, 'w' },
	{ { 1.0, 1.0, -1.0 }, r, 'r' },

	{ { -1.0, 1.0, 1.0 }, b, 'b' },
	{ { -1.0, 1.0, 1.0 }, w, 'w' },
	{ { -1.0, 1.0, 1.0 }, o, 'o' },

	{ { 1.0, 1.0, 1.0 }, r, 'r' },
	{ { 1.0, 1.0, 1.0 }, w, 'w' },
	{ { 1.0, 1.0, 1.0 }, b, 'b' },

	//lower edges
	{ { -1.0, -1.0, 0.0 }, y, 'y' },
	{ { -1.0, -1.0, 0.0 }, o, 'o' },

	{ { 0.0, -1.0, -1.0 }, y, 'y' },
	{ { 0.0, -1.0, -1.0 }, g, 'g' },

	{ { 1.0, -1.0, 0.0 }, y, 'y' },
	{ { 1.0, -1.0, 0.0 }, r, 'r' },

	{ { 0.0, -1.0, 1.0 }, y, 'y' },
	{ { 0.0, -1.0, 1.0 }, b, 'b' },

	//upper edges
	{ { -1.0, 1.0, 0.0 }, w, 'w' },
	{ { -1.0, 1.0, 0.0 }, o, 'o' },

	{ { 0.0, 1.0, -1.0 }, w, 'w' },
	{ { 0.0, 1.0, -1.0 }, g, 'g' },

	{ { 1.0, 1.0, 0.0 }, w, 'w' },
	{ { 1.0, 1.0, 0.0 }, r, 'r' },

	{ { 0.0, 1.0, 1.0 }, w, 'w' },
	{ { 0.0, 1.0, 1.0 }, b, 'b' },

	//middle edges
	{ { -1.0, 0.0, -1.0 }, g, 'g' },
	{ { -1.0, 0.0, -1.0 }, o, 'o' },

	{ { 1.0, 0.0, -1.0 }, r, 'r' },
	{ { 1.0, 0.0, -1.0 }, g, 'g' },

	{ { -1.0, 0.0, 1.0 }, o, 'o' },
	{ { -1.0, 0.0, 1.0 }, b, 'b' },

	{ { 1.0, 0.0, 1.0 }, r, 'r' },
	{ { 1.0, 0.0, 1.0 }, b, 'b' },

	//centers
	{ r, r, 'r' },
	{ w, w, 'w' },
	{ b, b, 'b' },
	{ o, o, 'o' },
	{ y, y, 'y' },
	{ g, g, 'g' },
};

Vector normalise(Vector v) {
	double x = v.x;
	double y = v.y;
	double z = v.z;

	double norm = sqrt(x*x + y * y + z * z);
	Vector res = { x / norm, y / norm, z / norm };
	return res;
}

Vector transform(Vector v)
{
	double inputMatrix[4][1];
	double outputMatrix[4][1];

	inputMatrix[0][0] = v.x;
	inputMatrix[1][0] = v.y;
	inputMatrix[2][0] = v.z;
	inputMatrix[3][0] = 1.0;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 1; j++) {
			outputMatrix[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				outputMatrix[i][j] += rotationMatrix[i][k] * inputMatrix[k][j];
			}
		}
	}
	Vector res = { outputMatrix[0][0], outputMatrix[1][0], outputMatrix[2][0] };
	return res;
}

void buildRotationMatrix(Vector axis, double theta)
{
	axis = normalise(axis);
	double u = axis.x;
	double v = axis.y;
	double w = axis.z;
	double u2 = u * u;
	double v2 = v * v;
	double w2 = w * w;

	rotationMatrix[0][0] = (u2 + (v2 + w2) * cos(theta));
	rotationMatrix[0][1] = (u * v * (1 - cos(theta)) - w * sin(theta));
	rotationMatrix[0][2] = (u * w * (1 - cos(theta)) + v * sin(theta));
	rotationMatrix[0][3] = 0.0;

	rotationMatrix[1][0] = (u * v * (1 - cos(theta)) + w * sin(theta));
	rotationMatrix[1][1] = (v2 + (u2 + w2) * cos(theta));
	rotationMatrix[1][2] = (v * w * (1 - cos(theta)) - u * sin(theta));
	rotationMatrix[1][3] = 0.0;

	rotationMatrix[2][0] = (u * w * (1 - cos(theta)) - v * sin(theta));
	rotationMatrix[2][1] = (v * w * (1 - cos(theta)) + u * sin(theta));
	rotationMatrix[2][2] = (w2 + (u2 + v2) * cos(theta));
	rotationMatrix[2][3] = 0.0;

	rotationMatrix[3][0] = 0.0;
	rotationMatrix[3][1] = 0.0;
	rotationMatrix[3][2] = 0.0;
	rotationMatrix[3][3] = 1.0;
}

int closestInt(double d) {
	int res = (int)d;
	if (d - (double)res < 0.5)
		return res;
	else
		return res + 1;
}

bool approx(Vector v1, Vector v2) {
	return ((abs(v1.x - v2.x) < EPSILON) && (abs(v1.y - v2.y) < EPSILON) && (abs(v1.z - v2.z) < EPSILON));
}

CubieFace cubie[3];
CubieFace ** findCubieAtPos(Vector pos) {
	for (int j = 0; j < 3; j++) {
		cubie[j] = (CubieFace){ {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 'x' };
	}
	int k = 0;
	for (int i = 0; i < 54; i++) {
		if (approx(cube[i].pos, pos)) {
			cubie[k] = cube[i];
			k++;
		}
	}
	CubieFace ** cubie_ptr = (CubieFace **)malloc(3);
	for (int l = 0; l < 3; l++) {
		cubie_ptr[l] = &cubie[l];
	}
	return cubie_ptr;
}

//returns the colour of the face of the cubie at the given pos and dir
char getCol(Vector pos, Vector dir) {
	char res = 'x';
	for (int i = 0; i < 54; i++) {
		if (approx(cube[i].pos, pos) && approx(cube[i].dir, dir))
			res = cube[i].col;
	}
	return res;
}

//true if v1 contains at least all of the non-zero elements of v2
bool contains(Vector v1, Vector v2) {
	return (
		((abs(v2.x) > EPSILON) && (abs(v1.x - v2.x) < EPSILON)) ||
		((abs(v2.y) > EPSILON) && (abs(v1.y - v2.y) < EPSILON)) ||
		((abs(v2.z) > EPSILON) && (abs(v1.z - v2.z) < EPSILON))
	);
}

void rotateFace(Vector axis, double theta) {
	buildRotationMatrix(axis, theta);
	for (int i = 0; i < 54; i++) {
		if (contains(cube[i].pos, axis)) {
			cube[i].pos = transform(cube[i].pos);
			cube[i].dir = transform(cube[i].dir);
		}
	}
}

int updateHeuristic() {
	int permsFromSolved = 0;
	for (int i = 0; i < 54; i++) {
		permsFromSolved += closestInt(abs(cube[i].pos.x - reference[i].pos.x));
		permsFromSolved += closestInt(abs(cube[i].pos.y - reference[i].pos.y));
		permsFromSolved += closestInt(abs(cube[i].pos.z - reference[i].pos.z));

		permsFromSolved += closestInt(abs(cube[i].dir.x - reference[i].dir.x));
		permsFromSolved += closestInt(abs(cube[i].dir.y - reference[i].dir.y));
		permsFromSolved += closestInt(abs(cube[i].dir.z - reference[i].dir.z));
	}
	return permsFromSolved;
}

void printCube() {
	/*
	      w w w
	      w w w
	      w w w
	o o o g g g r r r b b b
	o o o g g g r r r b b b
	o o o g g g r r r b b b
	      y y y
	      y y y
	      y y y
	*/
	printf("================================\n");
	printf("        %c %c %c\n",
		getCol((Vector){ -1.0, 1.0, 1.0 }, w), 
		getCol((Vector){ 0.0, 1.0, 1.0 }, w),
		getCol((Vector){ 1.0, 1.0, 1.0 }, w)
	);
	printf("        %c %c %c\n",
		getCol((Vector) { -1.0, 1.0, 0.0 }, w),
		getCol((Vector) { 0.0, 1.0, 0.0 }, w),
		getCol((Vector) { 1.0, 1.0, 0.0 }, w)
	);
	printf("        %c %c %c\n\n",
		getCol((Vector) { -1.0, 1.0, -1.0 }, w),
		getCol((Vector) { 0.0, 1.0, -1.0 }, w),
		getCol((Vector) { 1.0, 1.0, -1.0 }, w)
	);
	printf("%c %c %c   %c %c %c   %c %c %c   %c %c %c\n",
		getCol((Vector) { -1.0, 1.0, 1.0 }, o),
		getCol((Vector) { -1.0, 1.0, 0.0 }, o),
		getCol((Vector) { -1.0, 1.0, -1.0 }, o),

		getCol((Vector) { -1.0, 1.0, -1.0 }, g),
		getCol((Vector) { 0.0, 1.0, -1.0 }, g),
		getCol((Vector) { 1.0, 1.0, -1.0 }, g),

		getCol((Vector) { 1.0, 1.0, 1.0 }, r),
		getCol((Vector) { 1.0, 1.0, 0.0 }, r),
		getCol((Vector) { 1.0, 1.0, -1.0 }, r),

		getCol((Vector) { 1.0, 1.0, 1.0 }, b),
		getCol((Vector) { 0.0, 1.0, 1.0 }, b),
		getCol((Vector) { -1.0, 1.0, 1.0 }, b)
	);
	printf("%c %c %c   %c %c %c   %c %c %c   %c %c %c\n",
		getCol((Vector) { -1.0, 0.0, 1.0 }, o),
		getCol((Vector) { -1.0, 0.0, 0.0 }, o),
		getCol((Vector) { -1.0, 0.0, -1.0 }, o),

		getCol((Vector) { -1.0, 0.0, -1.0 }, g),
		getCol((Vector) { 0.0, 0.0, -1.0 }, g),
		getCol((Vector) { 1.0, 0.0, -1.0 }, g),

		getCol((Vector) { 1.0, 0.0, 1.0 }, r),
		getCol((Vector) { 1.0, 0.0, 0.0 }, r),
		getCol((Vector) { 1.0, 0.0, -1.0 }, r),

		getCol((Vector) { 1.0, 0.0, 1.0 }, b),
		getCol((Vector) { 0.0, 0.0, 1.0 }, b),
		getCol((Vector) { -1.0, 0.0, 1.0 }, b)
	);
	printf("%c %c %c   %c %c %c   %c %c %c   %c %c %c\n\n",
		getCol((Vector) { -1.0, -1.0, 1.0 }, o),
		getCol((Vector) { -1.0, -1.0, 0.0 }, o),
		getCol((Vector) { -1.0, -1.0, -1.0 }, o),

		getCol((Vector) { -1.0, -1.0, -1.0 }, g),
		getCol((Vector) { 0.0, -1.0, -1.0 }, g),
		getCol((Vector) { 1.0, -1.0, -1.0 }, g),

		getCol((Vector) { 1.0, -1.0, 1.0 }, r),
		getCol((Vector) { 1.0, -1.0, 0.0 }, r),
		getCol((Vector) { 1.0, -1.0, -1.0 }, r),

		getCol((Vector) { 1.0, -1.0, 1.0 }, b),
		getCol((Vector) { 0.0, -1.0, 1.0 }, b),
		getCol((Vector) { -1.0, -1.0, 1.0 }, b)
	);
	printf("        %c %c %c\n",
		getCol((Vector) { -1.0, -1.0, -1.0 }, y),
		getCol((Vector) { 0.0, -1.0, -1.0 }, y),
		getCol((Vector) { 1.0, -1.0, -1.0 }, y)
	);
	printf("        %c %c %c\n",
		getCol((Vector) { -1.0, -1.0, 0.0 }, y),
		getCol((Vector) { 0.0, -1.0, 0.0 }, y),
		getCol((Vector) { 1.0, -1.0, 0.0 }, y)
	);
	printf("        %c %c %c\n",
		getCol((Vector) { -1.0, -1.0, -1.0 }, y),
		getCol((Vector) { 0.0, -1.0, -1.0 }, y),
		getCol((Vector) { 1.0, -1.0, -1.0 }, y)
	);
	printf("================================\n");
}

char input[INPUTLEN];

void userInput() {
	fflush(stdout);
	fgets(input, sizeof(input), stdin);
	if (input[0] == '-') {
		switch (input[1]) {
		case 'p':
			printCube();
			break;
		case 'u':
			if (input[2] != 'p')
				rotateFace(w, PI / 2);
			else
				rotateFace(w, -PI / 2);
			break;
		case 'd':
			if (input[2] != 'p')
				rotateFace(y, PI / 2);
			else
				rotateFace(y, -PI / 2);
			break;
		case 'l':
			if (input[2] != 'p')
				rotateFace(o, PI / 2);
			else
				rotateFace(o, -PI / 2);
			break;
		case 'r':
			if (input[2] != 'p')
				rotateFace(r, PI / 2);
			else
				rotateFace(r, -PI / 2);
			break;
		case 'f':
			if (input[2] != 'p')
				rotateFace(g, PI / 2);
			else
				rotateFace(g, -PI / 2);
			break;
		case 'b':
			if (input[2] != 'p')
				rotateFace(b, PI / 2);
			else
				rotateFace(b, -PI / 2);
			break;
		default:
			printf("%s not a valid command\n", input);
		}
	}
	else {
		printf("%s not a valid command\n", input);
	}
}

int main()
{	
	printf("Welcome to ccubesolver: Here are some useful commands\n");
	printf("-p: prints the cube\n");
	printf("Cube Rotations:\n(Rotate a face clockwise (non-prime) or counter-clockwise (prime))\n");
	printf(" -u / -up: Up / Up prime\n -d / -dp: Down / Down prime\n -l / -lp: Left / Left prime\n -r / -rp: Right / Right prime\n -f / -fp: Front / Front prime\n -b / -bp: Back / Back prime\n");
	while (true) {
		printCube();
		printf("%d\n", updateHeuristic()/4);
		userInput();
	}
	return 0;
}
