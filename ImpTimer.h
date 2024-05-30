#ifndef IMP_TIMER_H_
#define IMP_TIMER_H_

class ImpTimer
{
public:
	ImpTimer();
	void start();
	void stop();
	void pause();
	void unpause();
	int get_ticks();
	bool started();
	bool paused();

private:
	int start_tick_;

	int paused_ticks_;

	bool paused_;
	bool started_;
};

#endif


