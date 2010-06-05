#include <game/client/component.h>

class CTeecompStats: public CComponent
{
private:
	int mode;
	int stats_cid;
	static void con_key_stats(void *result, void *user_data);
	static void con_key_next(void *result, void *user_data);
	void render_global_stats();
	void render_individual_stats();
	void check_stats_cid();

public:
	CTeecompStats();
	virtual void OnReset();
	virtual void OnMessage(int msgtype, void *rawmsg);
	bool is_active();
};

