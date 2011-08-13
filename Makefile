all: bot

bot: bot.c
	g++ -Wall -O3 bot.c -o bot

clean:
	rm -f bot