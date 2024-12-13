#include <stdlib.h>
#include <time.h>
const char *POEMS[] = {
    "老吾老，以及人之老；幼吾幼，以及人之幼。—— 孟子",
    "挟泰山以超北海，此不能也，非不为也；为老人折枝，是不为也，非不能也。—— "
    "庄子",
    "老来受尊敬，是人类精神最美好的一种特权。—— 司汤达",
    "将出牵衣送，未归倚阁望。—— 黄遵宪",
    "无情未必真豪杰，怜子如何不丈夫。—— 鲁迅",
    "天地之性，人为贵；人之行，莫大于孝，孝莫大于严父。——《孝经·圣至章》"
    "寸草春睴爱无私，孝心敬老代相传。",
    "人人亲其亲，长其长，而天下平。—— 孟子",
    "礼者，断长续短，损有余，益不足，达爱敬之文，而滋成行义之美也。—— 荀子",
    "祭而丰不如养之厚；悔之晚何若谨于前。——《格言联璧》",
    "尊老爱幼喜笑颜开，知足常乐永葆青春。",
    "扶老携幼扬美德，尊老爱少讲文明。",
    "构建和谐社会，弘扬敬老美德。",
    "文明家庭敬老为先，幸福之户以德为本。",
    "父母之年，不可不知也、一则以喜，一则以惧。——《论语》",
    "慈善行及至亲，但不应仅此为止。—— 富勒",
    "尊敬老人，不仅满足他们物质上的需要，更重要的，是对老人精神上的关照。",
    "老年，好比夜莺，应有他的夜曲。—— 康德",
    "年老受尊敬是出现在人类社会里的第一种特权。—— 拉法格",
    "我们体贴老人，要像对待孩子一样!—— 歌德",
    "动天之德莫大于孝，感物之道莫过于诚。—— 何铸",
    "侍于亲长，声容易肃，勿因琐事，大声呼叱。—— 周秉清",
    "亲善产生幸福，文明带来和谐。—— 雨果",
    "出入扶持须谨慎，朝夕伺候莫厌烦。—— 《劝报亲恩篇》"
    };

const int POEMS_SIZE = 23;

const char *random_poem() {
  // 初始化随机数生成器
  srand(time(NULL));
  // 生成一个随机索引
  int index = rand() % POEMS_SIZE;
  // 返回对应的诗句
  return POEMS[index];
}