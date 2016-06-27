struct Mouse {
	bool left;
	bool right;
	int x;
	int y;
};

Mouse mouse = {false, false, 0, 0};

GLfloat xAngle = 0.0f;
GLfloat yAngle = 0.0f;

void clicker(int button, int state, int x, int y) {
	// update the button state
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN)
			mouse.left = true;
		else
			mouse.left = false;
	}
	if(button == GLUT_RIGHT_BUTTON)
	{
		if(state == GLUT_DOWN)
			mouse.right = true;
		else
			mouse.right = false;
	}
	
	// update the mouse position, so we can track the mouse move
	mouse.x = x;
	mouse.y = y;
}

// Handler for mouse move event
void clickMove(int x, int y) {
	// Calculate the displacement in movement 
	int dx = mouse.x - x;
	int dy = mouse.y - y;

	// Commit the mouse position 
	mouse.x = x;
	mouse.y = y;
	
	// If left button is down, change the rotation angle for next frame 
	if(mouse.left) {
		xAngle -= dy * DPP;
		yAngle -= dx * DPP;
	}
	glutPostRedisplay();
}
