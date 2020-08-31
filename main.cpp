// [C++ SFML] doodle jump
// by.BlockDMask
// Blog : https://blockdmask.tistory.com/373
// Youtube : https://youtu.be/2fJ1_gTLDRE

#include<SFML/Graphics.hpp>
#include<ctime>
#include<vector>
using namespace sf;
using namespace std;

#define WIDTH 400		//가로
#define HEIGHT 600		//세로
#define BAR_COUNT 10	//밟는 bar 개수
static const float GRAVITY = 0.2f;	//중력

//플래이어 클래스
class Player
{
private:
	int x;
	int y;
	int imgWidth;
	int imgHeight;
	float dy;
	Sprite* imgJump;
	Sprite* imgReady;
	Texture t1;
	Texture t2;
	bool jumpFlag;
private:
	const Sprite& GetImg()
	{
		if (jumpFlag)
		{
			return *imgJump;
		}
		else
		{
			return *imgReady;
		}
	}

public:
	Player() : dy(0), jumpFlag(true)
	{
		x = static_cast<int>(WIDTH / 2);
		y = static_cast<int>(HEIGHT / 2);

		t1.loadFromFile("images/p1.png");
		t2.loadFromFile("images/p2.png");

		imgJump = new Sprite(t1);
		imgReady = new Sprite(t2);

		imgWidth = static_cast<int>(imgReady->getTexture()->getSize().x);
		imgHeight = static_cast<int>(imgReady->getTexture()->getSize().y);
	}
	~Player()
	{
		delete(imgJump);
		delete(imgReady);
	}

	void SetPosition()
	{
		imgReady->setPosition(x, y);
		imgJump->setPosition(x, y);
	}
	void Move()
	{
		if (Keyboard::isKeyPressed(Keyboard::Right)) //오른쪽이동
		{
			x += 4;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)) //왼쪽이동
		{
			x -= 4;
		}
		if (x <= 0)	//왼쪽 벽 뚫지 못하게
		{
			x = 0;
		}
		if (x >= WIDTH - imgWidth)	//오른쪽 벽 뚫지 못하게
		{
			x = WIDTH - imgWidth;
		}

		jumpFlag = true;
		dy += GRAVITY;
		y += static_cast<int>(dy);
		
		if (y > HEIGHT - imgHeight)
		{
			jumpFlag = false;
			dy = -10;
		}

	}
	void Draw(RenderWindow& window)
	{
		window.draw(GetImg());
	}

	float GetDy() const
	{
		return dy;
	}
	int GetY() const
	{
		return y;
	}
	int GetX() const
	{
		return x;
	}
	int GetWidth() const
	{
		return imgWidth;
	}
	int GetHeight() const
	{
		return imgHeight;
	}
	void SetY(int _y)
	{
		y = _y;
	}
	void Jump()
	{
		jumpFlag = false;
		dy = -10;
	}
};

//점프 bar 클래스
class Bar
{
private:
	struct Pos
	{
		int x;
		int y;
	};
	vector<Pos> vBar;
	Sprite* imgBar;
	Texture t;
	int imgWidth;
public:
	Bar()
	{
		srand(static_cast<unsigned int>(time(nullptr)));

		t.loadFromFile("images/bar.png");
		imgBar = new Sprite(t);

		imgWidth = imgBar->getTexture()->getSize().x;

		for (int i = 0; i < BAR_COUNT; ++i)
		{
			Pos p;
			p.x = rand() % WIDTH - imgWidth / 2;
			p.y = rand() % HEIGHT;
			vBar.push_back(p);
		}

		vBar[0].y = HEIGHT - 200;
	}
	~Bar()
	{
		delete(imgBar);
	}
	
	void Draw(RenderWindow& window)
	{
		for (int i = 0; i < BAR_COUNT; ++i)
		{
			imgBar->setPosition(vBar[i].x, vBar[i].y);
			window.draw(*imgBar);
		}
	}
	bool CheckCollision(Player* pPlayer)
	{
		//null check.
		if (pPlayer == nullptr)
		{
			return false;
		}

		for (int i = 0; i < BAR_COUNT; ++i)
		{
			if (pPlayer->GetDy() > 0
				&& pPlayer->GetX() + pPlayer->GetWidth() > vBar[i].x
				&& pPlayer->GetX() < vBar[i].x + imgWidth
				&& pPlayer->GetY() + pPlayer->GetHeight() > vBar[i].y
				&& pPlayer->GetY() + pPlayer->GetHeight() < vBar[i].y + 10)
			{
				pPlayer->Jump();
				return true;
			}
		}
		return false;
	}
	void MoveAndReset(Player* pPlayer)
	{
		static const int limit = HEIGHT / 3;
		if (pPlayer->GetY() < limit)
		{
			for (int i = 0; i < BAR_COUNT; ++i)
			{
				pPlayer->SetY(limit);
				vBar[i].y -= static_cast<int>(pPlayer->GetDy());
				if (vBar[i].y > HEIGHT + 10)
				{
					vBar[i].y = rand() % HEIGHT / 3 + 100;
					vBar[i].x = rand() % WIDTH;
				}
			}
		}
	}

};

int main(void)
{
	RenderWindow window(VideoMode(WIDTH, HEIGHT), "Doodle Game by.BlockDMask");
	window.setFramerateLimit(60);

	//setting
	Player* pPlayer = new Player();
	Bar* pBar = new Bar();

	while (window.isOpen())
	{
		Event e;
		if (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				window.close();
			}
		}
		//logic
		pPlayer->Move();
		pBar->MoveAndReset(pPlayer);
		pBar->CheckCollision(pPlayer);
		pPlayer->SetPosition();

		//draw
		window.clear(Color::White);
		pPlayer->Draw(window);
		pBar->Draw(window);
		window.display();
	}

	delete(pBar);
	delete(pPlayer);
	return 0;
}
