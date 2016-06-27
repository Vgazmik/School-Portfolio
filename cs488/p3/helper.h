

/*int angle = 0;

int dist(pt a, pt b) {
	float ret;
	float dx,dy,dz;
	
	dx = b.x-a.x;
	dy = b.y-a.y;
	dz = b.z-a.z;
	
	ret = sqrt((pow(dx,2.0) + pow(dy,2.0), pow(dz,2.0)));
	
	return ret;
}

void checker(pt lc, pt lf, pt rc, pt rf) {
	float factor = 4;
	float inc = 0.1;
	float t[4] = {0,0,0,0};
	//int cutH = dist(rf,rc) / factor;
	//int cutF = dist(rc,lc) / factor;
	line lv,bh,rv,th; 
	float ldx,ldy,ldz,bdx,bdy,bdz,rdx,rdy,rdz;
	float tdx,tdy,tdz;
	pt hold,h2,tmp;
        quad Q; // = {bl,tl,tr,br}, ^, >, v, <
	vector<quad> tiles;
	
      //     lf b--------------- rf
      //       /^              /^
      //      /lv             /rv
      //     / v             / v
      // lc /a___<-bh->____b/ rc
	
	lv.a = lc; lv.b = lf; //Define lines lv,bh,rv
	bh.a = lc; bh.b = rc;
	rv.a = rc; rv.b = rf;
	
	ldx = lv.b.x - lv.a.x; //Define coefficients 
	ldy = lv.b.y - lv.a.y; //for parametric form
	ldz = lv.b.z - lv.a.z; //of line
	
	rdx = rv.b.x - rv.a.x; //also rv
	rdy = rv.b.y - rv.a.y;
	rdz = rv.b.z - rv.a.z;
	
	bdx = bh.b.x - bh.a.x; //This line will be
	bdy = bh.b.y - bh.a.y; //updated
	bdz = bh.b.z - bh.a.z;
	
	Q.pts[BL] = lv.a; //starting position
	
	//calculate tl,tr,br
	//
	//first advance along lv,rv
	//lv for TL
	t[LV] = (dist(lv.a,lv.b)/factor)/inc; //won't change
	
	hold.x = Q.pts[BL].x + ldx*t[LV]; 
	hold.y = Q.pts[BL].y + ldy*t[LV]; 
	hold.z = Q.pts[BL].z + ldz*t[LV];
	
	Q.pts[TL] = hold;
	
	//now advance along rv
	t[RV] = (dist(rv.a,rv.b)/factor)/inc; //won't change
	
	hold.x = rc.x + rdx*t[RV]; 
	hold.y = rc.y + rdy*t[RV]; 
	hold.z = rc.z + rdz*t[RV];	  
	//keep hold to make line to TL
	th.a = Q.pts[TL]; 
	th.b = hold;     
	                 
	tdx = th.b.x - th.a.x;
	tdy = th.b.y - th.a.y;
	tdz = th.b.z - th.a.z;
	//solve for TR
	t[TH] = (dist(Q.pts[TL],hold)/factor)/inc;
	
	h2.x = Q.pts[TL].x + tdx*t[TH];
	h2.y = Q.pts[TL].y + tdy*t[TH];
	h2.z = Q.pts[TL].z + tdz*t[TH];
	
	Q.pts[TR] = h2;
	
	//finally, calculate BR
	t[BH] = (dist(bh.a,bh.b)/factor)/inc;
	
	h2.x = Q.pts[BL].x + bdx*t[BH];
	h2.y = Q.pts[BL].y + bdy*t[BH];
	h2.z = Q.pts[BL].z + bdz*t[BH];
	
	Q.pts[BR] = h2;
	
	tiles.push_back(Q);
	
	for(int j = 0; j < tiles.size(); j++) {
		Q = tiles[j];
		
		glBegin(GL_QUADS);
	 	  if(j%2 == 0)
		  	glColor3f(20.0f, 0.0f, 0.0f);
		  else
		  	glColor3f(1.0f, 20.0f, 1.0f);
		  tmp = Q.pts[BL];*/
		  /*cout << "\nBL-  x: " 
		       << tmp.x  
		       << "\ty: " 
		       << tmp.y  
		       << "\tz: " 
		       << tmp.z  
		       << endl;*/
	  	  //glVertex3f(tmp.x,tmp.y,tmp.z); // BL
	  	  //tmp = Q.pts[TL];
		  /*cout << "\nTL-  x: " 
		       << tmp.x  
		       << "\ty: " 
		       << tmp.y  
		       << "\tz: " 
		       << tmp.z  
		       << endl;*/
	  	  //glVertex3f(tmp.x,tmp.y,tmp.z); // TL
	  	  //tmp = Q.pts[TR];
		  /*cout << "\nTR-  x: " 
		       << tmp.x  
		       << "\ty: " 
		       << tmp.y  
		       << "\tz: " 
		       << tmp.z  
		       << endl;*/
	  	  //glVertex3f(tmp.x,tmp.y,tmp.z); // TR
	  	  //tmp = Q.pts[BR];
		  /*cout << "\nBR-  x: " 
		       << tmp.x  
		       << "\ty: " 
		       << tmp.y  
		       << "\tz: " 
		       << tmp.z  
		       << endl;*/
	  	  //glVertex3f(tmp.x,tmp.y,tmp.z); // BR
		//glEnd();
	//}
//}
