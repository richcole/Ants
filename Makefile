all: bot

bot: bot.c
	g++ -Wall -ggdb bot.c -o bot

clean:
	rm -f bot

run:
    # download and install http://pypi.python.org/pypi/ordereddict
    # download and unzip http://aichallengebeta.hypertriangle.com/tools.zip
	rm -rf game_logs
	cd tools && python2.6 playgame.py --engine_seed 41 --player_seed 41 --food none --end_wait=0.25 --verbose --log_dir game_logs --turns 30 --map_file submission_test/test.map ../bot "python submission_test/TestBot.py" -e --nolaunch --strict --capture_errors	
	cd tools && java -jar visualizer.jar game_logs/0.replay

