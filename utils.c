#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

int with_prefix(const char *str, const char *prefix)
{
  // 判断 str 是否以 prefix 开头
  // 对 strncmp 的封装
  return strncmp(str, prefix, strlen(prefix)) == 0;
}

void process_gbk_input(const char *gbk_str)
{
  // 在 Windows 上，scanf 输入进来的字符串用的是 gbk 编码，
  // 但是本程序默认是使用 utf8 输出，
  // 所以会员等的名字打出来是乱码
  // 在输入后，用此函数处理一遍输入好的 name
  // 即可转为 utf8 编码
#ifdef _WIN32
  int ori_len = MultiByteToWideChar(CP_ACP, 0, gbk_str, -1, NULL, 0);
  if (ori_len == 0)
  {
    // 转换失败，返回NULL
    return NULL;
  }

  wchar_t *wide_str = (wchar_t *)malloc(ori_len * sizeof(wchar_t));
  if (wide_str == NULL)
  {
    // 内存分配失败，返回NULL
    return NULL;
  }

  MultiByteToWideChar(CP_ACP, 0, gbk_str, -1, wide_str, ori_len);

  int converted_len = WideCharToMultiByte(CP_UTF8, 0, wide_str, -1, NULL, 0, NULL, NULL);
  if (converted_len == 0)
  {
    // 转换失败，释放已分配的内存并返回NULL
    free(wide_str);
    return NULL;
  }

  char *converted_str = (char *)malloc(converted_len * sizeof(char));
  if (converted_str == NULL)
  {
    // 内存分配失败，释放已分配的内存并返回NULL
    free(wide_str);
    return NULL;
  }

  WideCharToMultiByte(CP_UTF8, 0, wide_str, -1, converted_str, converted_len, NULL, NULL);

  free(wide_str);

  // return converted_str;
  strcpy(gbk_str, converted_str);
#endif
}

void remove_line_feed_chars(char *str)
{
  int length = strlen(str);
  for (int i = 0; i < length; i++)
  {
    if (str[i] == '\n')
    {
      str[i] = '\0';
      return;
    }
  }
}