float depth = 200;

void drawReflectiveSphere(){

  GLUquadric *quadric = gluNewQuadric();
  float sphere_pos[] = {10.0, 0.0, -40.0};

  glMatrixMode(GL_MODELVIEW);
  glDisable(GL_TEXTURE_2D);

  glPushMatrix();
    //Positive x
    gluLookAt(sphere_pos[0], sphere_pos[1], sphere_pos[2],
       sphere_pos[0] + 1, sphere_pos[1], sphere_pos[2],
       0.0, 1.0, 0.0);
    glCopyTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, -128, -128, 128, 128, 0);

    //Negative x
    gluLookAt(sphere_pos[0], sphere_pos[1], sphere_pos[2],
       sphere_pos[0] - 1, sphere_pos[1], sphere_pos[2],
       0.0, 1.0, 0.0);
    glCopyTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, -128, -128, 128, 128, 0);

    //Positive y
    gluLookAt(sphere_pos[0], sphere_pos[1], sphere_pos[2],
       sphere_pos[0], sphere_pos[1] + 1, sphere_pos[2],
       0.0, 0.0, 1.0);
    glCopyTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, -128, -128, 128, 128, 0);

    //Negative y
    gluLookAt(sphere_pos[0], sphere_pos[1], sphere_pos[2],
       sphere_pos[0], sphere_pos[1] - 1, sphere_pos[2],
       0.0, 0.0, 1.0);
    glCopyTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, -128, -128, 128, 128, 0);

    //Positive z
    gluLookAt(sphere_pos[0], sphere_pos[1], sphere_pos[2],
       sphere_pos[0], sphere_pos[1], sphere_pos[2] + 1,
       0.0, 1.0, 0.0);
    glCopyTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, -128, -128, 128, 128, 0);

    //Negative z
    gluLookAt(sphere_pos[0], sphere_pos[1], sphere_pos[2],
       sphere_pos[0], sphere_pos[1], sphere_pos[2] - 1,
       0.0, 1.0, 0.0);
    glCopyTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, -128, -128, 128, 128, 0);
  glPopMatrix();

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
  glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glEnable(GL_TEXTURE_GEN_R);
  glEnable(GL_TEXTURE_CUBE_MAP);

  glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(sphere_pos[0], sphere_pos[1], sphere_pos[2]);
    gluSphere(quadric, 5.0f, 128, 128);
  glPopMatrix();


  //Reset frustrum, etc.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, depth + 15.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_GEN_R);
  glDisable(GL_TEXTURE_CUBE_MAP);

  glDrawBuffer(GL_BACK);
  glReadBuffer(GL_BACK);
}
