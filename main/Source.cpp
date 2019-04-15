#include<iostream>
#include<GL/glut.h>
#include<math.h>

using namespace std;

double PI = 2 * acos(0);
double window_x = 640, window_y = 640;

struct point {
	float x, y;
};
struct velocity {
	float x, y;
};
struct point p1_y = { 0.0, 0.0 }, p2_g = { 60.0, 0.0 }, p3_outer = { 0.0, 0.0 };
double radius_y = 15.0, radius_g = 15.0;
double segnments_y = 24, segments_g = 24, segments_outer = 100;
double mass_y = 1, mass_g = 1;
struct velocity v1_y = { 1.0, 0.0 }, v2_g = { 0.0, 1.0 };
int front = 0, back = 0;
double outerCircleRadius = 150;


void drawCircle(GLfloat x, GLfloat y, GLfloat radius, int segments) {
	int i;
	GLfloat twicePi = 2.0f * PI;
	glBegin(GL_LINE_LOOP);
	for (i = 0; i <= segments; i++) {
		glVertex2f(
			x + (radius * cos(i *  twicePi / segments)),
			y + (radius* sin(i * twicePi / segments))
		);
	}
	glEnd();
}
double step = 0.0, xInc = 0.0 , yInc = 0.0, x = 0.0 , y = 0.0;
void drawLine(GLfloat p1_x, GLfloat p1_y, GLfloat p2_x, GLfloat p2_y) {
	glBegin(GL_LINES); {
		glVertex2f(p1_x, p1_y);
		glVertex2f(p2_x, p2_y);
	}
	glEnd();
}

float p2_x = 0.0, p2_y = 0.0, p3_x = 0.0, p3_y = 0.0, x_p3_p1 = 0.0;
void drawTriangle(GLfloat p1X, GLfloat p1Y, GLfloat cX, GLfloat cY) {
	
	double fraction = 0.3;
	struct velocity center_to_p_dir = {p1X - cX, p1Y - cY};
	struct point temp_p = {p1X - fraction * center_to_p_dir.x, p1Y - fraction * center_to_p_dir.y};
	struct point p2 = {temp_p.x + fraction * (cY - p1Y), temp_p.y + fraction * (p1X - cX)};
	struct point p3 = { temp_p.x - fraction * (cY - p1Y), temp_p.y - fraction * (p1X - cX) };
	glColor3f(1, 0, 0);
	glBegin(GL_TRIANGLES); 
	{
		glVertex2f(p1X, p1Y);
		glVertex2f(p2.x, p2.y);
		glVertex2f(p3.x, p3.y);
	}
	glEnd();
}

void drawCircle(double radius, int segments)
{
	int i;
	struct point points[100];
	glColor3f(0.7, 0.7, 0.7);
	//generate points
	for (i = 0; i <= segments; i++)
	{
		points[i].x = radius * cos(((double)i / (double)segments) * 2 * PI);
		points[i].y = radius * sin(((double)i / (double)segments) * 2 * PI);
	}
	//draw segments using generated points
	for (i = 0; i < segments; i++)
	{
		glBegin(GL_LINES);
		{
			glVertex3f(points[i].x, points[i].y, 0);
			glVertex3f(points[i + 1].x, points[i + 1].y, 0);
		}
		glEnd();
	}
}

//float angle = 0.0;


struct velocity rotate(struct velocity v, float angle) {
	return velocity{ v.x * cos(angle) - v.y * sin(angle), v.x * sin(angle) + v.y * cos(angle) };
}

void specialKeyListener(int key, int x, int y) {
	float angle = atan2(v1_y.y, v1_y.x);
	float v = sqrt(pow(v1_y.x, 2) + pow(v1_y.y, 2));
	switch (key) {
	case GLUT_KEY_RIGHT:
		angle -= .1;
		break;
	case GLUT_KEY_LEFT:
		angle += .1;
		break;
	default:
		break;
	}
	float v_x = v*cos(angle);
	float v_y = v*sin(angle);
	v1_y.x = v_x / sqrt(pow(v_x, 2) + pow(v_y, 2));
	v1_y.y = v_y / sqrt(pow(v_x, 2) + pow(v_y, 2));
}
void display() {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 200, 0, 0, 0, 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);
	glColor3f(1, 1, 1);
	drawCircle(p3_outer.x, p3_outer.y, outerCircleRadius, segments_outer);
	//yellow ball...p1, v1
	glColor3f(1, 1, 0);
	drawCircle(p1_y.x, p1_y.y, radius_y, segnments_y);
	//green ball...p2, v2
	glColor3f(0, 1, 0);
	drawCircle(p2_g.x, p2_g.y, radius_g, segments_g);
	//yellow_dir_line
	glColor3f(1, 1, 1);
	drawLine(p1_y.x, p1_y.y, p1_y.x + radius_y*cos(atan2(v1_y.y, v1_y.x)), p1_y.y + radius_y * sin(atan2(v1_y.y, v1_y.x)));
	//green_dir_line
	drawLine(p2_g.x, p2_g.y, p2_g.x + radius_g * cos(atan2(v2_g.y, v2_g.x)), p2_g.y + radius_y * sin(atan2(v2_g.y, v2_g.x)));
	//yellow triangle
	drawTriangle(p1_y.x + radius_y * cos(atan2(v1_y.y, v1_y.x)), p1_y.y + radius_y * sin(atan2(v1_y.y, v1_y.x)), p1_y.x, p1_y.y);
	//green_triangle
	drawTriangle(p2_g.x + radius_g * cos(atan2(v2_g.y, v2_g.x)), p2_g.y + radius_y * sin(atan2(v2_g.y, v2_g.x)), p2_g.x, p2_g.y);
	//drawCircle(30, 24);
	glutSwapBuffers();
}




void resolveCollision(struct point circle1, struct point circle2, struct velocity circle1_v, struct velocity circle2_v) {
	float xVelocityDiff = circle1_v.x - circle2_v.x;
	float yVelocityDiff = circle1_v.y - circle2_v.y;
	float xDist = circle2.x - circle1.x;
	float yDist = circle2.y - circle1.y;
	if (xVelocityDiff * xDist + yVelocityDiff * yDist >= 0) {
		float angle = -atan2(yDist, xDist);
		struct velocity u1 = rotate(circle1_v, angle);
		struct velocity u2 = rotate(circle2_v, angle);
		struct velocity v1 = {(u1.x * (mass_y - mass_g))/(mass_y + mass_g) + (u2.x * 2 * mass_g)/(mass_y + mass_g), u1.y};
		struct velocity v2 = { (u2.x * (mass_y - mass_g)) / (mass_y + mass_g) + (u1.x * 2 * mass_g) / (mass_y + mass_g), u2.y };
		struct velocity vFinal_1 = rotate(v1, -angle);
		struct velocity vFinal_2 = rotate(v2, -angle);
		v1_y.x = vFinal_1.x / sqrt(pow(vFinal_1.x, 2) + pow(vFinal_1.y, 2));
		v1_y.y = vFinal_1.y / sqrt(pow(vFinal_1.x, 2) + pow(vFinal_1.y, 2));
		v2_g.x = vFinal_2.x / sqrt(pow(vFinal_2.x, 2) + pow(vFinal_2.y, 2));
		v2_g.y = vFinal_2.y / sqrt(pow(vFinal_2.x, 2) + pow(vFinal_2.y, 2));
	}
}
void animate() {
	double centerDist = sqrt(pow(p2_g.x - p1_y.x, 2) + pow(p2_g.y - p1_y.y, 2));
	double d_y_p3 = sqrt(pow(p1_y.x - p3_outer.x, 2) + pow(p1_y.y - p3_outer.y, 2));
	double d_g_p3 = sqrt(pow(p2_g.x - p3_outer.x, 2) + pow(p2_g.y - p3_outer.y, 2));
	if (centerDist < 2*radius_y) {
		cout << "INNER CIRCLE COLLIDES" << endl;
		struct point circle1 = { p1_y.x, p1_y.y };
		struct point circle2 = { p2_g.x, p2_g.y };
		struct velocity circle1_v = { v1_y.x, v1_y.y };
		struct velocity circle2_v = { v2_g.x, v2_g.y };
		resolveCollision(circle1, circle2, circle1_v, circle2_v);
	}
	if (d_y_p3 > 9 * radius_y) {
		double collsionDist = sqrt(pow(p1_y.x - p3_outer.x, 2) + pow(p1_y.y - p3_outer.y, 2));
		double n_x = (p3_outer.x - p1_y.x) / collsionDist;
		double n_y = (p3_outer.y - p1_y.y) / collsionDist;
		cout << "BEFORE: Y_x: " << v1_y.x << " Y_y: " << v1_y.y << endl;
		float r_v_x = v1_y.x - 2 * n_x * (v1_y.x * n_x + v1_y.y * n_y);
		float r_v_y = v1_y.y - 2 * n_y * (v1_y.x * n_x + v1_y.y * n_y);
		v1_y.x = r_v_x / sqrt(pow(r_v_x, 2) + pow(r_v_y, 2));
		v1_y.y = r_v_y / sqrt(pow(r_v_x, 2) + pow(r_v_y, 2));
		cout << "AFTER: Y_x: " << v1_y.x << " Y_y: " << v1_y.y << endl;
	}

	if (d_g_p3 > 9 * radius_g) {
		double collsionDist = sqrt(pow(p2_g.x - p3_outer.x, 2) + pow(p2_g.y - p3_outer.y, 2));
		double n_x = (p3_outer.x - p2_g.x) / collsionDist;
		double n_y = (p3_outer.y - p2_g.y) / collsionDist;
		cout << "BEFORE: G_x: " << v2_g.x << " G_y: " << v2_g.y << endl;
		double reflected_x = v2_g.x - 2 * n_x * (v2_g.x * n_x + v2_g.y * n_y);
		double reflected_y = v2_g.y - 2 * n_y * (v2_g.x * n_x + v2_g.y * n_y);
		v2_g.x = reflected_x / sqrt(pow(reflected_x, 2) + pow(reflected_y, 2));
		v2_g.y = reflected_y / sqrt(pow(reflected_x, 2) + pow(reflected_y, 2));
		cout << "AFTER: G_x: " << v2_g.x << " G_y: " << v2_g.y << endl;
	}
	p1_y.x += v1_y.x;
	p1_y.y += v1_y.y;
	p2_g.x += v2_g.x;
	p2_g.y += v2_g.y;
	glutPostRedisplay();
}
void init() {
	glClearColor(0, 0, 0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80, 1, 1, 1000.0);
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(window_x, window_y);
	glutCreateWindow("Bubbles");
	init();
	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(display);
	glutIdleFunc(animate);
	glutSpecialFunc(specialKeyListener);
	glutMainLoop();
}