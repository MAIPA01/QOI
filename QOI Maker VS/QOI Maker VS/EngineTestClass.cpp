#include "EngineTestClass.h"
#include <list>

void EngineTestClass::TestTriangles()
{
	SDL_Color white = { 0xFF, 0xFF, 0xFF, 0xFF };
	SDL_Color green = { 0x00, 0xFF, 0x00, 0xFF };

	struct Triangle {
		Point2D point1;
		Point2D point2;
		Point2D point3;
	};

	std::vector<Triangle> testTriangles = std::vector<Triangle>();
	testTriangles.push_back({ { 90, 100 }, { 200, 400 }, { 630, 300 } }); // 1
	testTriangles.push_back({ { 90, 100 }, { 90, 400 }, { 630, 300 } }); // 2
	testTriangles.push_back({ { 90, 100 }, { 630, 400 }, { 630, 300 } }); // 3
	testTriangles.push_back({ { 90, 100 }, { 200, 300 }, { 630, 300 } }); // 4
	testTriangles.push_back({ { 90, 100 }, { 200, 50 }, { 630, 50 } }); // 5
	for (int y = 220; y <= 330; y += 10) { // (1 - 12)
        for (int x = 55; x <= 115; x += 10) { // (1 - 7)
            testTriangles.push_back({ { 100, (float)y }, { (float)x, 250 }, { 70, 300 } });
        }
	}

	for (int i = 0; i < testTriangles.size(); i++) {
		LWindowPtr window = loadWindow("TestTriangle: " + std::to_string(i+1), 640, 480);
		window->init();
		window->clear();
		window->DrawFilledTriangle(testTriangles[i].point1, testTriangles[i].point2, testTriangles[i].point3, white);
		window->DrawTriangle(testTriangles[i].point1, testTriangles[i].point2, testTriangles[i].point3, green);
		window->update();
	}
}

void EngineTestClass::TestCircle()
{
	SDL_Color white = { 0xFF, 0xFF, 0xFF, 0xFF };
	SDL_Color green = { 0x00, 0xFF, 0x00, 0xFF };

	LWindowPtr window = loadWindow("TestCircle: " + std::to_string(0), 640, 480);
	window->init();
	window->clear();
	window->DrawFilledCircle({ 320, 240 }, 200, white);
	window->DrawCircle({ 320, 240 }, 200, green);
	window->update();
}

void EngineTestClass::OnStart()
{
    pDepthBuffer = new float[getMainWindow()->getWidth() * getMainWindow()->getHeight()];
	meshCube.TestCube();
	matProj = Matrix_MakeProjection(90.0f, (float)getMainWindow()->getHeight() / (float)getMainWindow()->getWidth(), 0.1f, 1000.0f);

	getMainWindow()->clear();
	SDL_Color white = { 0xFF, 0xFF, 0xFF, 0xFF };
	SDL_Color green = { 0x00, 0xFF, 0x00, 0xFF };
	SDL_Color red = { 0xFF, 0x00, 0x00, 0xFF };
	
	//TestTriangles();
	//TestCircle();

    LTexturePtr textureTest = loadTexture("./Tutorial/35_window_events/window.png");
    LButtonPtr buttonTest = addButton({ 320, 35 }, 100, 50, textureTest);
    LTextPtr textTest = addText({ 0,0 }, "TEST", 25, green, "./Tutorial/16_true_type_fonts/lazy.ttf", buttonTest->getPosition());

    TextureRenderData data;
    data.clip->w = buttonTest->getSize().x;
    data.clip->h = buttonTest->getSize().y;
    getMainWindow()->DrawButton(buttonTest, data);
    getMainWindow()->DrawText(textTest, TextureRenderData());
}

void EngineTestClass::OnUpdate(float deltaTime)
{
    //Test3D(deltaTime);
}

void EngineTestClass::OnEvent(SDL_Event& e)
{
}

void EngineTestClass::OnDestroy()
{
}

void EngineTestClass::Test3D(float deltaTime)
{
    SDL_Color white = { 0xFF, 0xFF, 0xFF, 0xFF };
    SDL_Color green = { 0x00, 0xFF, 0x00, 0xFF };
    vec3d vForward = Vector_Mul(vLookDir, 8.0f * deltaTime);

    mat4x4 matRotZ, matRotX;
    fTheta += 1.0f * deltaTime;

    matRotZ = Matrix_MakeRotationZ(fTheta * 0.5f);
    matRotX = Matrix_MakeRotationX(fTheta);

    mat4x4 matTrans;
    matTrans = Matrix_MakeTranslation(0.0f, 0.0f, 2.5f);

    mat4x4 matWorld;
    matWorld = Matrix_MakeIdentity();
    matWorld = Matrix_MultiplyMatrix(matRotZ, matRotX);
    matWorld = Matrix_MultiplyMatrix(matWorld, matTrans);

    vec3d vUp = { 0, 1, 0 };
    vec3d vTarget = { 0, 0, 1 };
    mat4x4 matCameraRot = Matrix_MakeRotationY(fYaw);
    vLookDir = Matrix_MultiplyVector(matCameraRot, vTarget);
    vTarget = Vector_Add(vCamera, vLookDir);

    mat4x4 matCamera = Matrix_PointAt(vCamera, vTarget, vUp);

    mat4x4 matView = Matrix_QuickInverse(matCamera);

    std::vector<triangle> vecTrianglesToRaster;

    // Draw Triangles
    for (auto tri : meshCube.tris)
    {
        triangle triProjected, triTransformed, triViewed;

        triTransformed.p[0] = Matrix_MultiplyVector(matWorld, tri.p[0]);
        triTransformed.p[1] = Matrix_MultiplyVector(matWorld, tri.p[1]);
        triTransformed.p[2] = Matrix_MultiplyVector(matWorld, tri.p[2]);
        triTransformed.t[0] = tri.t[0];
        triTransformed.t[1] = tri.t[1];
        triTransformed.t[2] = tri.t[2];

        vec3d normal, line1, line2;

        line1 = Vector_Sub(triTransformed.p[1], triTransformed.p[0]);
        line2 = Vector_Sub(triTransformed.p[2], triTransformed.p[0]);

        normal = Vector_CrossProduct(line1, line2);

        normal = Vector_Normalise(normal);

        vec3d vCameraRay = Vector_Sub(triTransformed.p[0], vCamera);

        if (Vector_DotProduct(normal, vCameraRay) < 0.0f)
        {
            // Ilumination
            vec3d light_direction = { 0.0f, 1.0f, -1.0f };
            light_direction = Vector_Normalise(light_direction);

            float dp = max(0.1f, Vector_DotProduct(light_direction, normal));

            triTransformed.color = white;

            triViewed.p[0] = Matrix_MultiplyVector(matView, triTransformed.p[0]);
            triViewed.p[1] = Matrix_MultiplyVector(matView, triTransformed.p[1]);
            triViewed.p[2] = Matrix_MultiplyVector(matView, triTransformed.p[2]);
            triViewed.color = triTransformed.color;
            triViewed.t[0] = triTransformed.t[0];
            triViewed.t[1] = triTransformed.t[1];
            triViewed.t[2] = triTransformed.t[2];

            int nClippedTriangles = 0;
            triangle clipped[2];
            nClippedTriangles = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

            for (int n = 0; n < nClippedTriangles; n++)
            {
                // Project triangles from 3D --> 2D
                triProjected.p[0] = Matrix_MultiplyVector(matProj, clipped[n].p[0]);
                triProjected.p[1] = Matrix_MultiplyVector(matProj, clipped[n].p[1]);
                triProjected.p[2] = Matrix_MultiplyVector(matProj, clipped[n].p[2]);
                triProjected.color = clipped[n].color;
                triProjected.t[0] = clipped[n].t[0];
                triProjected.t[1] = clipped[n].t[1];
                triProjected.t[2] = clipped[n].t[2];



                triProjected.t[0].u = triProjected.t[0].u / triProjected.p[0].w;
                triProjected.t[1].u = triProjected.t[1].u / triProjected.p[1].w;
                triProjected.t[2].u = triProjected.t[2].u / triProjected.p[2].w;

                triProjected.t[0].v = triProjected.t[0].v / triProjected.p[0].w;
                triProjected.t[1].v = triProjected.t[1].v / triProjected.p[1].w;
                triProjected.t[2].v = triProjected.t[2].v / triProjected.p[2].w;

                triProjected.t[0].w = 1.0f / triProjected.p[0].w;
                triProjected.t[1].w = 1.0f / triProjected.p[1].w;
                triProjected.t[2].w = 1.0f / triProjected.p[2].w;


                triProjected.p[0] = Vector_Div(triProjected.p[0], triProjected.p[0].w);
                triProjected.p[1] = Vector_Div(triProjected.p[1], triProjected.p[1].w);
                triProjected.p[2] = Vector_Div(triProjected.p[2], triProjected.p[2].w);

                triProjected.p[0].x *= -1.0f;
                triProjected.p[1].x *= -1.0f;
                triProjected.p[2].x *= -1.0f;
                triProjected.p[0].y *= -1.0f;
                triProjected.p[1].y *= -1.0f;
                triProjected.p[2].y *= -1.0f;

                // Offset verts into visible normalised space
                vec3d vOffsetView = { 1, 1, 0 };
                triProjected.p[0] = Vector_Add(triProjected.p[0], vOffsetView);
                triProjected.p[1] = Vector_Add(triProjected.p[1], vOffsetView);
                triProjected.p[2] = Vector_Add(triProjected.p[2], vOffsetView);

                triProjected.p[0].x *= 0.5f * (float)getMainWindow()->getWidth();
                triProjected.p[0].y *= 0.5f * (float)getMainWindow()->getHeight();
                triProjected.p[1].x *= 0.5f * (float)getMainWindow()->getWidth();
                triProjected.p[1].y *= 0.5f * (float)getMainWindow()->getHeight();
                triProjected.p[2].x *= 0.5f * (float)getMainWindow()->getWidth();
                triProjected.p[2].y *= 0.5f * (float)getMainWindow()->getHeight();

                // Store triangle for sorting
                vecTrianglesToRaster.push_back(triProjected);
            }
        }
    }

    // Sort triangles from back to front
    /*sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle& t1, triangle& t2)
        {
            float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
            float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
            return z1 > z2; });*/

    getMainWindow()->clear();

    for (int i = 0; i < getMainWindow()->getWidth() * getMainWindow()->getHeight(); i++) {
        pDepthBuffer[i] = 0.0f;
    }

    for (auto& triToRaster : vecTrianglesToRaster)
    {

        triangle clipped[2];
        std::list<triangle> listTriangles;
        listTriangles.push_back(triToRaster);
        int nNewTriangles = 1;

        for (int p = 0; p < 4; p++)
        {
            int nTrisToAdd = 0;
            while (nNewTriangles > 0)
            {
                triangle test = listTriangles.front();
                listTriangles.pop_front();
                nNewTriangles--;

                switch (p)
                {
                case 0:
                    nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]);
                    break;
                case 1:
                    nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, (float)getMainWindow()->getHeight() - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]);
                    break;
                case 2:
                    nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]);
                    break;
                case 3:
                    nTrisToAdd = Triangle_ClipAgainstPlane({ (float)getMainWindow()->getWidth() - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]);
                    break;
                }

                for (int w = 0; w < nTrisToAdd; w++)
                    listTriangles.push_back(clipped[w]);
            }
            nNewTriangles = listTriangles.size();
        }
        for (auto& t : listTriangles)
        {
            /*TexturedTriangle(t.p[0].x, t.p[0].y, t.t[0].u, t.t[0].v, t.t[0].w,
                t.p[1].x, t.p[1].y, t.t[1].u, t.t[1].v, t.t[1].w,
                t.p[2].x, t.p[2].y, t.t[2].u, t.t[2].v, t.t[2].w, sprTex1);*/
            getMainWindow()->DrawFilledTriangle({ t.p[0].x, t.p[0].y }, { t.p[1].x, t.p[1].y }, { t.p[2].x, t.p[2].y }, white);
            getMainWindow()->DrawTriangle({ t.p[0].x, t.p[0].y }, { t.p[1].x , t.p[1].y }, { t.p[2].x, t.p[2].y }, green);
        }
    }
}

EngineTestClass::EngineTestClass()
{
}

EngineTestClass::~EngineTestClass()
{
}
