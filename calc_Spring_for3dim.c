#include "Ce_MPS.h"

void calc_Spring_for3dim(void){
  int i, j, k, l, s, s_old, t;
  double xij, yij, zij, distance;
  double n1[3], n2[3], a[3], b[3], c[3], N1, N2, theta;
  int p[4], q[4], tmp, pa[3], pb[3];

  // spring for streching
  for(i=0; i<num_t; i++){
    if((pcl[tri[i].p1].isIgn == YES) || (pcl[tri[i].p2].isIgn == YES) || (pcl[tri[i].p3].isIgn == YES)) continue;
    p[0] = pcl[tri[i].p1].num;
    p[1] = pcl[tri[i].p2].num;
    p[2] = pcl[tri[i].p3].num;
    p[3] = pcl[tri[i].p1].num;

    for(j=0; j<2; j++){
      // calculate normal vector (xij, yij, zij)
      distance = calDistance(p[j], p[j+1]);
      xij = (Position[p[j+1]*3+X] - Position[p[j]*3+X]) / distance;
      yij = (Position[p[j+1]*3+Y] - Position[p[j]*3+Y]) / distance;
      zij = (Position[p[j+1]*3+Z] - Position[p[j]*3+Z]) / distance;

      Acceleration[p[j]*3+X]   += SPRING_CONSTANT * (distance - meanWallDistance) * xij / 2;
      Acceleration[p[j]*3+Y]   += SPRING_CONSTANT * (distance - meanWallDistance) * yij / 2;
      Acceleration[p[j]*3+Z]   += SPRING_CONSTANT * (distance - meanWallDistance) * zij / 2;
      Acceleration[p[j+1]*3+X] -= SPRING_CONSTANT * (distance - meanWallDistance) * xij / 2;
      Acceleration[p[j+1]*3+Y] -= SPRING_CONSTANT * (distance - meanWallDistance) * yij / 2;
      Acceleration[p[j+1]*3+Z] -= SPRING_CONSTANT * (distance - meanWallDistance) * zij / 2;
    }
  }

  // spring for bending
  for(i=0; i<num_t; i++){
    // 20171106
    // adjacentTriangleを用いて隣接三角形間のバネ計算を書く
    for(j=0; j<3; j++){
      s = 0;
      t = 0;
      if(adjacentTriangle[i][j] == -1) continue;
      pa[0] = pcl[tri[i].p1].num;
      pa[1] = pcl[tri[i].p2].num;
      pa[2] = pcl[tri[i].p3].num;
      pb[0] = pcl[tri[adjacentTriangle[i][j]].p1].num;
      pb[1] = pcl[tri[adjacentTriangle[i][j]].p2].num;
      pb[2] = pcl[tri[adjacentTriangle[i][j]].p3].num;

      // acquire the pair particles as p[0], p[1], and the others q[0], q[1]
      for(k=0; k<3; k++){
        s_old = s;
        for(l=0; l<3; l++){
          if(pa[k] == pb[l]){
            p[s] = pa[k];
            s++;
            break;
          }
        }
        if(s == s_old){
          q[t] = pa[k];
          t++;
        }
      }

      s = 0;
      for(k=0; k<3; k++){
        s_old = s;
        for(l=0; l<3; l++){
          if(pb[k] == pa[l]){
            s++;
            break;
          }
        }
        if(s == s_old){
          q[t] = pb[k];
        }
      }

      // 粒子iから各粒子への位置ベクトルa,b,cを求める
      a[X] = Position[p[1]*3+X] - Position[p[0]*3+X];
      a[Y] = Position[p[1]*3+Y] - Position[p[0]*3+Y];
      a[Z] = Position[p[1]*3+Z] - Position[p[0]*3+Z];
      b[X] = Position[q[0]*3+X] - Position[p[0]*3+X];
      b[Y] = Position[q[0]*3+Y] - Position[p[0]*3+Y];
      b[Z] = Position[q[0]*3+Z] - Position[p[0]*3+Z];
      c[X] = Position[q[1]*3+X] - Position[p[0]*3+X];
      c[Y] = Position[q[1]*3+Y] - Position[p[0]*3+Y];
      c[Z] = Position[q[1]*3+Z] - Position[p[0]*3+Z];

      // 外積を用いて法線ベクトルn1,n2を求め正規化
      n1[X] = a[Y]*b[Z] - a[Z]*b[Y];
      n1[Y] = a[Z]*b[X] - a[X]*b[Z];
      n1[Z] = a[X]*b[Y] - a[Y]*b[X];
      n2[X] = - a[Y]*c[Z] + a[Z]*c[Y];
      n2[Y] = - a[Z]*c[X] + a[X]*c[Z];
      n2[Z] = - a[X]*c[Y] + a[Y]*c[X];
      N1 = sqrt(n1[X]*n1[X] + n1[Y]*n1[Y] + n1[Z]*n1[Z]);
      N2 = sqrt(n2[X]*n2[X] + n2[Y]*n2[Y] + n2[Z]*n2[Z]);
      if(N1 != 0){
        n1[X] /= N1;
        n1[Y] /= N1;
        n1[Z] /= N1;
      }
      if(N2 != 0){
        n2[X] /= N2;
        n2[Y] /= N2;
        n2[Z] /= N2;
      }

      // n1とn2の内積を利用して2つの三角形の成す角thetaを求める
      theta = M_PI - acos(fmin(fmax(n1[X]*n2[X] + n1[Y]*n2[Y] + n1[Z]*n2[Z], -1), 1));
      if(theta == 0){
        tmp = p[1];
        p[1] = p[2];
        p[2] = tmp;

        // 粒子iから各粒子への位置ベクトルa,b,cを求める
        a[X] = Position[p[1]*3+X] - Position[p[0]*3+X];
        a[Y] = Position[p[1]*3+Y] - Position[p[0]*3+Y];
        a[Z] = Position[p[1]*3+Z] - Position[p[0]*3+Z];
        b[X] = Position[q[0]*3+X] - Position[p[0]*3+X];
        b[Y] = Position[q[0]*3+Y] - Position[p[0]*3+Y];
        b[Z] = Position[q[0]*3+Z] - Position[p[0]*3+Z];
        c[X] = Position[q[1]*3+X] - Position[p[0]*3+X];
        c[Y] = Position[q[1]*3+Y] - Position[p[0]*3+Y];
        c[Z] = Position[q[1]*3+Z] - Position[p[0]*3+Z];

        // 外積を用いて法線ベクトルn1,n2を求め正規化
        n1[X] = a[Y]*b[Z] - a[Z]*b[Y];
        n1[Y] = a[Z]*b[X] - a[X]*b[Z];
        n1[Z] = a[X]*b[Y] - a[Y]*b[X];
        n2[X] = a[Y]*c[Z] - a[Z]*c[Y];
        n2[Y] = a[Z]*c[X] - a[X]*c[Z];
        n2[Z] = a[X]*c[Y] - a[Y]*c[X];
        N1 = sqrt(n1[X]*n1[X] + n1[Y]*n1[Y] + n1[Z]*n1[Z]);
        N2 = sqrt(n2[X]*n2[X] + n2[Y]*n2[Y] + n2[Z]*n2[Z]);
        if(N1 != 0){
          n1[X] /= N1;
          n1[Y] /= N1;
          n1[Z] /= N1;
        }
        if(N2 != 0){
          n2[X] /= N2;
          n2[Y] /= N2;
          n2[Z] /= N2;
        }

        // n1とn2の内積を利用して2つの三角形の成す角thetaを求める
        theta = M_PI - acos(fmin(fmax(n1[X]*n2[X] + n1[Y]*n2[Y] + n1[Z]*n2[Z], -1), 1));

        /* printf("theta(re) = %e\n", theta); */
        /* printf("a = (%e, %e, %e)\n", a[X], a[Y], a[Z]); */
        /* printf("b = (%e, %e, %e)\n", b[X], b[Y], b[Z]); */
        /* printf("c = (%e, %e, %e)\n", c[X], c[Y], c[Z]); */
        /* printf("n1 = (%e, %e, %e)\n", n1[X], n1[Y], n1[Z]); */
        /* printf("n2 = (%e, %e, %e)\n", n2[X], n2[Y], n2[Z]); */
        /* printf("\n"); */
      }


      // 力を求める
      Acceleration[q[0]*3+X] += 0.5 * BENDING_CONSTANT * tan((M_PI - theta)/2) * n1[X];
      Acceleration[q[0]*3+Y] += 0.5 * BENDING_CONSTANT * tan((M_PI - theta)/2) * n1[Y];
      Acceleration[q[0]*3+Z] += 0.5 * BENDING_CONSTANT * tan((M_PI - theta)/2) * n1[Z];
      Acceleration[q[1]*3+X] += 0.5 * BENDING_CONSTANT * tan((M_PI - theta)/2) * n2[X];
      Acceleration[q[1]*3+Y] += 0.5 * BENDING_CONSTANT * tan((M_PI - theta)/2) * n2[Y];
      Acceleration[q[1]*3+Z] += 0.5 * BENDING_CONSTANT * tan((M_PI - theta)/2) * n2[Z];

      Acceleration[p[0]*3+X] -= 0.5 * (BENDING_CONSTANT * tan((M_PI - theta)/2) * (n1[X] + n2[X])) / 2;
      Acceleration[p[0]*3+Y] -= 0.5 * (BENDING_CONSTANT * tan((M_PI - theta)/2) * (n1[Y] + n2[Y])) / 2;
      Acceleration[p[0]*3+Z] -= 0.5 * (BENDING_CONSTANT * tan((M_PI - theta)/2) * (n1[Z] + n2[Z])) / 2;
      Acceleration[p[1]*3+X] -= 0.5 * (BENDING_CONSTANT * tan((M_PI - theta)/2) * (n1[X] + n2[X])) / 2;
      Acceleration[p[1]*3+Y] -= 0.5 * (BENDING_CONSTANT * tan((M_PI - theta)/2) * (n1[Y] + n2[Y])) / 2;
      Acceleration[p[1]*3+Z] -= 0.5 * (BENDING_CONSTANT * tan((M_PI - theta)/2) * (n1[Z] + n2[Z])) / 2;
    }
  }
}
