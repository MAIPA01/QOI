#pragma once

#include "SimpleGameEngine.h"
#include <fstream>
#include <strstream>

template<class T>
T max(T a, T b) {
    if (a > b)
        return a;
    return b;
}

struct vec2d {
    float u = 0;
    float v = 0;
    float w = 0;
};

struct vec3d
{
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 1;
};

struct triangle
{
    vec3d p[3];
    vec2d t[3];

    SDL_Color color;
};

struct mesh
{
    std::vector<triangle> tris;

    bool LoadFromObjectFile(std::string sFilename, bool bHasTexture = false)
    {

        std::ifstream f(sFilename);
        if (!f.is_open())
            return false;

        // Local cache of verts
        std::vector<vec3d> verts;
        std::vector<vec2d> texs;

        while (!f.eof())
        {
            char line[128];
            f.getline(line, 128);

            std::strstream s;
            s << line;

            char junk;

            if (line[0] == 'v')
            {
                if (line[1] == 't') {
                    vec2d v;
                    s >> junk >> junk >> v.u >> v.v;
                    texs.push_back(v);
                }
                else {
                    vec3d v;
                    s >> junk >> v.x >> v.y >> v.z;
                    verts.push_back(v);
                }
            }

            if (!bHasTexture) {
                if (line[0] == 'f')
                {
                    int f[3];
                    s >> junk >> f[0] >> f[1] >> f[2];
                    tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
                }
            }
            else {
                if (line[0] == 'f') {
                    s >> junk;

                    std::string tokens[6];
                    int nTokenCount = -1;

                    while (!s.eof()) {
                        char c = s.get();
                        if (c == ' ' || c == '/') {
                            nTokenCount++;
                        }
                        else {
                            tokens[nTokenCount].append(1, c);
                        }
                    }

                    tokens[nTokenCount].pop_back();

                    tris.push_back({ verts[stoi(tokens[0]) - 1], verts[stoi(tokens[2]) - 1], verts[stoi(tokens[4]) - 1],
                        texs[stoi(tokens[1]) - 1], texs[stoi(tokens[3]) - 1], texs[stoi(tokens[5]) - 1] });
                }
            }
        }

        return true;
    }

    void TestCube()
    {
        tris = {

            // SOUTH
            {0.0f, 0.0f, 0.0f, 1.0f,      0.0f, 1.0f, 0.0f, 1.0f,     1.0f, 1.0f, 0.0f, 1.0f,       0.0f, 1.0f, 1.0f,     0.0f, 0.0f, 1.0f,     1.0f, 0.0f, 1.0f },
            {0.0f, 0.0f, 0.0f, 1.0f,      1.0f, 1.0f, 0.0f, 1.0f,     1.0f, 0.0f, 0.0f, 1.0f,       0.0f, 1.0f, 1.0f,     1.0f, 0.0f, 1.0f,     1.0f, 1.0f, 1.0f },

            // EAST                                                                          
            {1.0f, 0.0f, 0.0f, 1.0f,      1.0f, 1.0f, 0.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f,       0.0f, 1.0f, 1.0f,     0.0f, 0.0f, 1.0f,     1.0f, 0.0f, 1.0f },
            {1.0f, 0.0f, 0.0f, 1.0f,      1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f, 1.0f, 1.0f,       0.0f, 1.0f, 1.0f,     1.0f, 0.0f, 1.0f,     1.0f, 1.0f, 1.0f },

            // NORTH                                                                                      
            {1.0f, 0.0f, 1.0f, 1.0f,      1.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 1.0f, 1.0f,       0.0f, 1.0f, 1.0f,     0.0f, 0.0f, 1.0f,     1.0f, 0.0f, 1.0f },
            {1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 0.0f, 1.0f, 1.0f,       0.0f, 1.0f, 1.0f,     1.0f, 0.0f, 1.0f,     1.0f, 1.0f, 1.0f },

            // WEST                                                                                     
            {0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f, 1.0f,       0.0f, 1.0f, 1.0f,     0.0f, 0.0f, 1.0f,     1.0f, 0.0f, 1.0f },
            {0.0f, 0.0f, 1.0f, 1.0f,      0.0f, 1.0f, 0.0f, 1.0f,     0.0f, 0.0f, 0.0f, 1.0f,       0.0f, 1.0f, 1.0f,     1.0f, 0.0f, 1.0f,     1.0f, 1.0f, 1.0f },

            // TOP                                                                                           
            {0.0f, 1.0f, 0.0f, 1.0f,      0.0f, 1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f,       0.0f, 1.0f, 1.0f,     0.0f, 0.0f, 1.0f,     1.0f, 0.0f, 1.0f },
            {0.0f, 1.0f, 0.0f, 1.0f,      1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 0.0f, 1.0f,       0.0f, 1.0f, 1.0f,     1.0f, 0.0f, 1.0f,     1.0f, 1.0f, 1.0f },

            // BOTTOM                                                                                  
            {1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 0.0f, 0.0f, 1.0f,       0.0f, 1.0f, 1.0f,     0.0f, 0.0f, 1.0f,     1.0f, 0.0f, 1.0f },
            {1.0f, 0.0f, 1.0f, 1.0f,      0.0f, 0.0f, 0.0f, 1.0f,     1.0f, 0.0f, 0.0f, 1.0f,       0.0f, 1.0f, 1.0f,     1.0f, 0.0f, 1.0f,     1.0f, 1.0f, 1.0f },

        };
    }
};

struct mat4x4
{
    float m[4][4] = { 0 };
};

class EngineTestClass : public SimpleGameEngine
{
private:
    mesh meshCube;
    mat4x4 matProj;
    vec3d vCamera;
    vec3d vLookDir;
    float fYaw;
    float fTheta;

    vec3d Matrix_MultiplyVector(mat4x4& m, vec3d& i)
    {
        vec3d v;
        v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
        v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
        v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
        v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
        return v;
    }

    mat4x4 Matrix_MakeIdentity()
    {
        mat4x4 matrix;
        matrix.m[0][0] = 1.0f;
        matrix.m[1][1] = 1.0f;
        matrix.m[2][2] = 1.0f;
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    mat4x4 Matrix_MakeRotationX(float fAngleRad)
    {
        mat4x4 matrix;
        matrix.m[0][0] = 1.0f;
        matrix.m[1][1] = cosf(fAngleRad);
        matrix.m[1][2] = sinf(fAngleRad);
        matrix.m[2][1] = -sinf(fAngleRad);
        matrix.m[2][2] = cosf(fAngleRad);
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    mat4x4 Matrix_MakeRotationY(float fAngleRad)
    {
        mat4x4 matrix;
        matrix.m[0][0] = cosf(fAngleRad);
        matrix.m[0][2] = sinf(fAngleRad);
        matrix.m[2][0] = -sinf(fAngleRad);
        matrix.m[1][1] = 1.0f;
        matrix.m[2][2] = cosf(fAngleRad);
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    mat4x4 Matrix_MakeRotationZ(float fAngleRad)
    {
        mat4x4 matrix;
        matrix.m[0][0] = cosf(fAngleRad);
        matrix.m[0][1] = sinf(fAngleRad);
        matrix.m[1][0] = -sinf(fAngleRad);
        matrix.m[1][1] = cosf(fAngleRad);
        matrix.m[2][2] = 1.0f;
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    mat4x4 Matrix_MakeTranslation(float x, float y, float z)
    {
        mat4x4 matrix;
        matrix.m[0][0] = 1.0f;
        matrix.m[1][1] = 1.0f;
        matrix.m[2][2] = 1.0f;
        matrix.m[3][3] = 1.0f;
        matrix.m[3][0] = x;
        matrix.m[3][1] = y;
        matrix.m[3][2] = z;
        return matrix;
    }

    mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
    {
        float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
        mat4x4 matrix;
        matrix.m[0][0] = fAspectRatio * fFovRad;
        matrix.m[1][1] = fFovRad;
        matrix.m[2][2] = fFar / (fFar - fNear);
        matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
        matrix.m[2][3] = 1.0f;
        matrix.m[3][3] = 0.0f;
        return matrix;
    }

    mat4x4 Matrix_MultiplyMatrix(mat4x4& m1, mat4x4& m2)
    {
        mat4x4 matrix;
        for (int c = 0; c < 4; c++)
            for (int r = 0; r < 4; r++)
                matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
        return matrix;
    }

    mat4x4 Matrix_PointAt(vec3d& pos, vec3d& target, vec3d& up)
    {
        vec3d newForward = Vector_Sub(target, pos);
        newForward = Vector_Normalise(newForward);

        vec3d a = Vector_Mul(newForward, Vector_DotProduct(up, newForward));
        vec3d newUp = Vector_Sub(up, a);
        newUp = Vector_Normalise(newUp);

        vec3d newRight = Vector_CrossProduct(newUp, newForward);

        mat4x4 matrix;
        matrix.m[0][0] = newRight.x;
        matrix.m[0][1] = newRight.y;
        matrix.m[0][2] = newRight.z;
        matrix.m[0][3] = 0.0f;
        matrix.m[1][0] = newUp.x;
        matrix.m[1][1] = newUp.y;
        matrix.m[1][2] = newUp.z;
        matrix.m[1][3] = 0.0f;
        matrix.m[2][0] = newForward.x;
        matrix.m[2][1] = newForward.y;
        matrix.m[2][2] = newForward.z;
        matrix.m[2][3] = 0.0f;
        matrix.m[3][0] = pos.x;
        matrix.m[3][1] = pos.y;
        matrix.m[3][2] = pos.z;
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    mat4x4 Matrix_QuickInverse(mat4x4& m)
    {
        mat4x4 matrix;
        matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
        matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
        matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
        matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
        matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
        matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    vec3d Vector_Add(vec3d& v1, vec3d& v2)
    {
        return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
    }

    vec3d Vector_Sub(vec3d& v1, vec3d& v2)
    {
        return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
    }

    vec3d Vector_Mul(vec3d& v1, float k)
    {
        return { v1.x * k, v1.y * k, v1.z * k };
    }

    vec3d Vector_Div(vec3d& v1, float k)
    {
        return { v1.x / k, v1.y / k, v1.z / k };
    }

    float Vector_DotProduct(vec3d& v1, vec3d& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    float Vector_Length(vec3d& v)
    {
        return sqrtf(Vector_DotProduct(v, v));
    }

    vec3d Vector_Normalise(vec3d& v)
    {
        float l = Vector_Length(v);
        return { v.x / l, v.y / l, v.z / l };
    }

    vec3d Vector_CrossProduct(vec3d& v1, vec3d& v2)
    {
        vec3d v;
        v.x = v1.y * v2.z - v1.z * v2.y;
        v.y = v1.z * v2.x - v1.x * v2.z;
        v.z = v1.x * v2.y - v1.y * v2.x;
        return v;
    }

    vec3d Vector_IntersectPlane(vec3d& plane_p, vec3d& plane_n, vec3d& lineStart, vec3d& lineEnd, float& t)
    {
        plane_n = Vector_Normalise(plane_n);
        float plane_d = -Vector_DotProduct(plane_n, plane_p);
        float ad = Vector_DotProduct(lineStart, plane_n);
        float bd = Vector_DotProduct(lineEnd, plane_n);
        t = (-plane_d - ad) / (bd - ad);
        vec3d lineStartToEnd = Vector_Sub(lineEnd, lineStart);
        vec3d lineToIntersect = Vector_Mul(lineStartToEnd, t);
        return Vector_Add(lineStart, lineToIntersect);
    }

    int Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle& in_tri, triangle& out_tri1, triangle& out_tri2)
    {
        plane_n = Vector_Normalise(plane_n);

        auto dist = [&](vec3d& p)
        {
            vec3d n = Vector_Normalise(p);
            return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - Vector_DotProduct(plane_n, plane_p));
        };

        vec3d* inside_points[3]; int nInsidePointCount = 0;
        vec3d* outside_points[3]; int nOutsidePointCount = 0;

        vec2d* inside_tex[3]; int nInsideTexCount = 0;
        vec2d* outside_tex[3]; int nOutsideTexCount = 0;

        float d0 = dist(in_tri.p[0]);
        float d1 = dist(in_tri.p[1]);
        float d2 = dist(in_tri.p[2]);

        if (d0 >= 0)
        {
            inside_points[nInsidePointCount++] = &in_tri.p[0];
            inside_tex[nInsideTexCount++] = &in_tri.t[0];
        }
        else
        {
            outside_points[nOutsidePointCount++] = &in_tri.p[0];
            outside_tex[nOutsideTexCount++] = &in_tri.t[0];
        }
        if (d1 >= 0)
        {
            inside_points[nInsidePointCount++] = &in_tri.p[1];
            inside_tex[nInsideTexCount++] = &in_tri.t[1];
        }
        else
        {
            outside_points[nOutsidePointCount++] = &in_tri.p[1];
            outside_tex[nOutsideTexCount++] = &in_tri.t[1];
        }
        if (d2 >= 0)
        {
            inside_points[nInsidePointCount++] = &in_tri.p[2];
            inside_tex[nInsideTexCount++] = &in_tri.t[2];
        }
        else
        {
            outside_points[nOutsidePointCount++] = &in_tri.p[2];
            outside_tex[nOutsideTexCount++] = &in_tri.t[2];
        }

        if (nInsidePointCount == 0)
        {
            return 0;
        }

        if (nInsidePointCount == 3)
        {
            out_tri1 = in_tri;
            return 1;
        }

        if (nInsidePointCount == 1 && nOutsidePointCount == 2)
        {
            out_tri1.color = in_tri.color;

            out_tri1.p[0] = *inside_points[0];
            out_tri1.t[0] = *inside_tex[0];

            float t;
            out_tri1.p[1] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
            out_tri1.t[1].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
            out_tri1.t[1].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;


            out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1], t);
            out_tri1.t[2].u = t * (outside_tex[1]->u - inside_tex[0]->u) + inside_tex[0]->u;
            out_tri1.t[2].v = t * (outside_tex[1]->v - inside_tex[0]->v) + inside_tex[0]->v;

            return 1;
        }

        if (nInsidePointCount == 2 && nOutsidePointCount == 1)
        {
            out_tri1.color = in_tri.color;

            out_tri2.color = in_tri.color;

            out_tri1.p[0] = *inside_points[0];
            out_tri1.p[1] = *inside_points[1];
            out_tri1.t[0] = *inside_tex[0];
            out_tri1.t[1] = *inside_tex[1];

            float t;
            out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
            out_tri1.t[2].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
            out_tri1.t[2].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;

            out_tri2.p[0] = *inside_points[1];
            out_tri2.p[1] = out_tri1.p[2];
            out_tri2.t[0] = *inside_tex[1];
            out_tri2.t[1] = out_tri1.t[2];

            out_tri2.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0], t);
            out_tri2.t[2].u = t * (outside_tex[0]->u - inside_tex[1]->u) + inside_tex[1]->u;
            out_tri2.t[2].v = t * (outside_tex[0]->v - inside_tex[1]->v) + inside_tex[1]->v;

            return 2;
        }
    }

    /*CHAR_INFO GetColour(float lum)
    {
        short bg_col, fg_col;
        wchar_t sym;
        int pixel_bw = (int)(13.0f * lum);
        switch (pixel_bw)
        {
        case 0: bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID; break;

        case 1: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_QUARTER; break;
        case 2: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_HALF; break;
        case 3: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_THREEQUARTERS; break;
        case 4: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_SOLID; break;

        case 5: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_QUARTER; break;
        case 6: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_HALF; break;
        case 7: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_THREEQUARTERS; break;
        case 8: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_SOLID; break;

        case 9:  bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_QUARTER; break;
        case 10: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_HALF; break;
        case 11: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_THREEQUARTERS; break;
        case 12: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_SOLID; break;
        default:
            bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID;
        }

        CHAR_INFO c;
        c.Attributes = bg_col | fg_col;
        c.Char.UnicodeChar = sym;
        return c;
    }*/

    float* pDepthBuffer = nullptr;

	void TestTriangles();
	void TestCircle();
	virtual void OnStart();
	virtual void OnUpdate(float deltaTime);
	virtual void OnEvent(SDL_Event& e);
	virtual void OnDestroy();
    void Test3D(float deltaTime);

public:
	EngineTestClass();
	virtual ~EngineTestClass();
};

