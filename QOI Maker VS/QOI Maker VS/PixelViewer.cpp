#include "PixelViewer.h"
#include "include/Model/qoi.h"
#include "include/Exceptions/qoi_exceptions.h"

void TestButtonCall() {
	printf("Clicked XD\n");
}

void PixelViewer::OnStart()
{
	gTexture = loadTexture("./Tutorial/35_window_events/window.png");

	buttonText = addText({0,0}, "Load QOI", 28, { 0x00, 0x00, 0x00, 0x00 }, "./Tutorial/16_true_type_fonts/lazy.ttf");
	testButton = addButton({ getMainWindow()->getWidth() / 2.0f - buttonText->getWidth() / 2.0f, getMainWindow()->getHeight() / 2.0f - buttonText->getHeight() / 2.0f }, 100, 100, buttonText->getTexture());

	std::vector<std::string> files;
	files.push_back("./qoi_test_images/dice.qoi");
	pixelsShowWindows = std::vector<LWindowPtr>(files.size() + 1);
	imagesPixels = std::vector<pixels>(files.size() + 1);

	int i = 0;
	for (auto& fileName : files) {
		LButtonCall f = [this, fileName, i]() -> void {
			printf("Openning %s file...\n", fileName.c_str());
			bool decoded = false;
			QOI qoi_image;
			try {
				qoi_image.decode(fileName);
				printf("Decoded.\n");
				decoded = true;

				/*std::fstream dice_bin_file("./diceBin.txt", std::ios_base::out);
				dice_bin_file << FileBytes(fileName);
				dice_bin_file.close();*/
			}
			catch (Exception& e) {
				printf("%s", e.GetMessage().c_str());
			}

			if (decoded) {
				imagesPixels[i].clear();
				imagesPixels[i] = qoi_image.getPixels();
				if (pixelsShowWindows[i] == nullptr) {
					pixelsShowWindows[i] = loadWindow(fileName, qoi_image.getWidth(), qoi_image.getHeight());
					pixelsShowWindows[i]->init(false);
				}

				pixelsShowWindows[i]->focus();

				bool encoded = false;
				try {
					qoi_image.encode("./test.qoi");
					printf("Encoded.\n");
					encoded = true;

					/*std::fstream test_bin_file("./testBin.txt", std::ios_base::out);
					test_bin_file << FileBytes("./test.qoi");
					test_bin_file.close();*/
				}
				catch (Exception& e) {
					printf("%s", e.GetMessage().c_str());
				}

				if (encoded) {
					decoded = false;
					try {
						qoi_image.decode("./test.qoi");
						printf("Decoded.\n");
						decoded = true;
					}
					catch (Exception& e) {
						printf("%s", e.GetMessage().c_str());
					}

					if (decoded) {
						imagesPixels[imagesPixels.size() - 1].clear();
						imagesPixels[imagesPixels.size() - 1] = qoi_image.getPixels();
						if (pixelsShowWindows[imagesPixels.size() - 1] == nullptr) {
							pixelsShowWindows[imagesPixels.size() - 1] = loadWindow("./test.qoi", qoi_image.getWidth(), qoi_image.getHeight());
							pixelsShowWindows[imagesPixels.size() - 1]->init(false);
						}

						pixelsShowWindows[imagesPixels.size() - 1]->focus();
					}
				}
			}
		};
		testButton->addCallOnClick(f);
		++i;
	}
	testButton->addCallOnClick(TestButtonCall);
}

void PixelViewer::OnEvent(SDL_Event& e)
{
}

void PixelViewer::OnUpdate(float deltaTime)
{
	TextureRenderData data;
	data.pos.x = getMainWindow()->getWidth() / 2;
	data.pos.y = getMainWindow()->getHeight() / 2;
	//getMainWindow()->DrawTexture(gTexture, data);
	data.clip = new SDL_Rect();
	data.clip->w = 150;
	data.clip->h = 20;
	getMainWindow()->DrawButton(testButton, data);

	int i = 0;
	for (auto& window : pixelsShowWindows) {
		if (window != nullptr) {
			for (int y = 0; y < window->getHeight(); y++) {
				for (int x = 0; x < window->getWidth(); x++) {
					PixelPtr curr_pix = imagesPixels[i][y * window->getWidth() + x];
					window->DrawPixel({ (float)x, (float)y }, {curr_pix->r, curr_pix->g, curr_pix->b, curr_pix->a});
				}
			}
		}
		++i;
	}
}

void PixelViewer::OnDestroy()
{
}

PixelViewer::PixelViewer()
{
}

PixelViewer::~PixelViewer()
{
}

/*#include "Pixel_Exceptions.h"

//Number of available devices
int gRecordingDeviceCount = 0;

//Recieved audio spec
SDL_AudioSpec gReceivedRecordingSpec;
SDL_AudioSpec gReceivedPlaybackSpec;

//Recording data buffer
Uint8* gRecordingBuffer = NULL;

//Size of data buffer
Uint32 gBufferByteSize = 0;

//Position in data buffer
Uint32 gBufferBytePosition = 0;

//Maximum position in data buffer for recording
Uint32 gBufferByteMaxPosition = 0;

PixelViewer::PixelViewer() {};

PixelViewer::~PixelViewer() {};

void PixelViewer::SetPixels(const Pixel* newPixels, unsigned int length)
{
	if (this->pixels != nullptr)
		delete this->pixels;
	this->pixels = new Pixel[length];
	memcpy(this->pixels, newPixels, sizeof(Pixel) * length);
}

void PixelViewer::LoadMedia()
{
	gWindow = gWindows[0];
	gRenderer = gWindow.getRenderer();

	//Set starting prompt 
	//gPromptTexture.loadFromRenderedText("Select your recording device:", {0x00, 0x00, 0x00, 0xFF}, gRenderer, gFont);

	//Get capture device count
	/*gRecordingDeviceCount = SDL_GetNumAudioDevices(SDL_TRUE);

	//No recording devices
	if (gRecordingDeviceCount < 1)
	{
		printf("Unable to get audio capture device! SDL Error: %s\n", SDL_GetError());
	}
	//At least one device connected
	else
	{
		//Cap recording device count
		if (gRecordingDeviceCount > MAX_RECORDING_DEVICES)
		{
			gRecordingDeviceCount = MAX_RECORDING_DEVICES;
		}

		//Render device names
		std::stringstream promptText;
		for (int i = 0; i < gRecordingDeviceCount; ++i)
		{
			//Get capture device name
			promptText.str("");
			promptText << i << ": " << SDL_GetAudioDeviceName(i, SDL_TRUE);

			//Set texture from name
			gDeviceTextures[i].loadFromRenderedText(promptText.str().c_str(), {0x00, 0x00, 0x00, 0xFF}, gRenderer, gFont);
		}
	}*/

	//Open file for reading in binary
	/*SDL_RWops* file = SDL_RWFromFile("./Tutorial/33_file_reading_and_writing/nums.bin", "r+b");
	//File does not exist
	if (file == NULL)
	{
		printf("Warning: Unable to open file! SDL Error: %s\n", SDL_GetError());

		//Create file for writing
		file = SDL_RWFromFile("./Tutorial/33_file_reading_and_writing/nums.bin", "w+b");
		if (file != NULL)
		{
			printf("New file created!\n");

			//Initialize data
			for (int i = 0; i < TOTAL_DATA; ++i)
			{
				gData[i] = 0;
				SDL_RWwrite(file, &gData[i], sizeof(Sint32), 1);
			}

			//Close file handler
			SDL_RWclose(file);
		}
		else
		{
			printf("Error: Unable to create file! SDL Error: %s\n", SDL_GetError());
		}
	}
	//File exists
	else
	{
		//Load data
		printf("Reading file...!\n");
		for (int i = 0; i < TOTAL_DATA; ++i)
		{
			SDL_RWread(file, &gData[i], sizeof(Sint32), 1);
		}

		//Close file handler
		SDL_RWclose(file);
	}*/
/*}

void PixelViewer::Init(std::string windowName, unsigned int screen_width, unsigned int screen_height)
{

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		std::string s(SDL_GetError());
		throw PixelViewerException("SDL could not initialize! SDL_Error: " + s);
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Get number of displays
		gTotalDisplays = SDL_GetNumVideoDisplays();
		if (gTotalDisplays < 2)
		{
			printf("Warning: Only one display connected!");
		}

		//Get bounds of each display
		gDisplayBounds = new SDL_Rect[gTotalDisplays];
		for (int i = 0; i < gTotalDisplays; ++i)
		{
			SDL_GetDisplayBounds(i, &gDisplayBounds[i]);
		}

		//Create window
		if (!gWindows[0].init(screen_width, screen_height))
		{
			printf("Window 0 could not be created!\n");
		}
		else
		{
			//Create renderer for window
			/*gRenderer = gWindow.createRenderer();
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
				}

				//Initialize SDL_mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
				}

				this->screen_width = screen_width;
				this->screen_height = screen_height;
			}*/
		/*}
	}
}

void PixelViewer::Close()
{
	//Open data for writing
	SDL_RWops* file = SDL_RWFromFile("./Tutorial/33_file_reading_and_writing/nums.bin", "w+b");
	if (file != NULL)
	{
		//Save data
		for (int i = 0; i < TOTAL_DATA; ++i)
		{
			SDL_RWwrite(file, &gData[i], sizeof(Sint32), 1);
		}

		//Close file handler
		SDL_RWclose(file);
	}
	else
	{
		printf("Error: Unable to save file! %s\n", SDL_GetError());
	}

	//Free loaded images
	gSceneTexture.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Destroy window    
	SDL_DestroyRenderer(gRenderer);
	//Destroy windows
	for (int i = 0; i < TOTAL_WINDOWS; ++i)
	{
		gWindows[i].free();
	}
	gRenderer = NULL;

	//Free playback audio
	if (gRecordingBuffer != NULL)
	{
		delete[] gRecordingBuffer;
		gRecordingBuffer = NULL;
	}

	//Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();

	delete[] pixels;
}

void PixelViewer::EventLoop() {

	//Initialize the rest of the windows
	/*for (int i = 1; i < TOTAL_WINDOWS; ++i)
	{
		gWindows[i].init(screen_width, screen_height);
	}*/

	/*bool quit = false;

	SDL_Event e;

	//Set the default recording state
	RecordingState currentState = SELECTING_DEVICE;

	//Audio device IDs
	SDL_AudioDeviceID recordingDeviceId = 0;
	SDL_AudioDeviceID playbackDeviceId = 0;

	//The dot that will be moving around on the screen
	Dot dot(gRedTexture, gGreenTexture, gBlueTexture);

	//While application is running
	//While application is running
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			//Handle input for the dot
			dot.handleEvent(e);
		}

		//Move the dot
		dot.move(screen_width, screen_height);

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		//Render objects
		dot.render(&gDotTexture, gRenderer, 0, 0, gShimmerTexture, gRedTexture, gGreenTexture, gBlueTexture);

		//Update screen
		SDL_RenderPresent(gRenderer);
	}
}

bool PixelViewer::checkCollision(Circle& a, Circle& b)
{
	//Calculate total radius squared
	int totalRadiusSquared = a.r + b.r;
	totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

	//If the distance between the centers of the circles is less than the sum of their radii
	if (distanceSquared(a.x, a.y, b.x, b.y) < (totalRadiusSquared))
	{
		//The circles have collided
		return true;
	}

	//If not
	return false;
}

bool PixelViewer::checkCollision(Circle& a, SDL_Rect& b)
{
	//Closest point on collision box
	int cX, cY;

	//Find closest x offset
	if (a.x < b.x)
	{
		cX = b.x;
	}
	else if (a.x > b.x + b.w)
	{
		cX = b.x + b.w;
	}
	else
	{
		cX = a.x;
	}
	//Find closest y offset
	if (a.y < b.y)
	{
		cY = b.y;
	}
	else if (a.y > b.y + b.h)
	{
		cY = b.y + b.h;
	}
	else
	{
		cY = a.y;
	}

	//If the closest point is inside the circle
	if (distanceSquared(a.x, a.y, cX, cY) < a.r * a.r)
	{
		//This box and the circle have collided
		return true;
	}

	//If the shapes have not collided
	return false;
}

double PixelViewer::distanceSquared(int x1, int y1, int x2, int y2)
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return deltaX * deltaX + deltaY * deltaY;
}

void audioRecordingCallback(void* userdata, Uint8* stream, int len)
{
	//Copy audio from stream
	memcpy(&gRecordingBuffer[gBufferBytePosition], stream, len);

	//Move along buffer
	gBufferBytePosition += len;
}

void audioPlaybackCallback(void* userdata, Uint8* stream, int len)
{
	//Copy audio to stream
	memcpy(stream, &gRecordingBuffer[gBufferBytePosition], len);

	//Move along buffer
	gBufferBytePosition += len;
}
*/
