import configparser

def create_gameevent_ini():
    config = configparser.ConfigParser()
    config.optionxform = str
    
    events = [
        {
            "Name": "2",
            "EventType": "0", # ET_INYEAR
            "EventEffect": "0", # EE_NONE
            "EventValue": "0",
            "TalkCount": "1",
            "Talk0_Pic": "shiqiang.bmp",
            "Talk0_Talker": "史强",
            "Talk0_Content": "古筝行动非常成功！我们拿到了伊文斯在审判日号上的三体通讯记录。"
        },
        {
            "Name": "10",
            "EventType": "0", 
            "EventEffect": "0", 
            "EventValue": "0",
            "TalkCount": "1",
            "Talk0_Pic": "sophon.bmp",
            "Talk0_Talker": "智子",
            "Talk0_Content": "你们是虫子！基础科学的枷锁已经锁死！"
        },
        {
            "Name": "50",
            "EventType": "0", 
            "EventEffect": "6", # EE_MOON_CRISIS
            "EventValue": "0",
            "TalkCount": "1",
            "Talk0_Pic": "luoji.bmp",
            "Talk0_Talker": "最高统帅部",
            "Talk0_Content": "月球危机爆发！由于过度开采和潮汐引力异常，月球即将解体坠落！我们需要立即调集所有资源推开月球碎片！"
        },
        {
            "Name": "300",
            "EventType": "0", 
            "EventEffect": "7", # EE_WANDERING_EARTH
            "EventValue": "0",
            "TalkCount": "1",
            "Talk0_Pic": "wangmiao.bmp",
            "Talk0_Talker": "联合政府",
            "Talk0_Content": "太阳系即将毁灭。如果我们掌握了重核聚变行星发动机，我们将开启流浪地球计划！"
        }
    ]
    
    config['count'] = {'count': str(len(events))}
    
    for i, event in enumerate(events):
        config[str(i)] = event
        
    with open('gameevent.ini', 'w', encoding='gbk') as f:
        config.write(f)

if __name__ == '__main__':
    create_gameevent_ini()
    print("gameevent.ini generated successfully in GBK.")
