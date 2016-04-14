/*
 * gVirtuS -- A GPGPU transparent virtualization component.
 *
 * Copyright (C) 2009-2010  The University of Napoli Parthenope at Naples.
 *
 * This file is part of gVirtuS.
 *
 * gVirtuS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * gVirtuS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gVirtuS; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Written by: Giuseppe Coviello <giuseppe.coviello@uniparthenope.it>,
 *             Department of Applied Science
 */

/**
 * @file   ConfigFile.cpp
 * @author Giuseppe Coviello <giuseppe.coviello@uniparthenope.it>
 * @date   Thu Oct 1 12:56:07 2009
 *
 * @brief
 *
 *
 */

#include "ConfigFile.h"

#include <stdio.h>

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>

using namespace std;

int ConfigFile::backend_gpu = 1;	//默认只使用一个后端GPU

/*
 * 该函数找到输入字符串中第一个为'#'的字符并且将其设置为'\0'
 */

void eatcomments(char *s) {
    char *comments = strchr(s, '#');
    if(comments)
        s[comments - s] = 0;
}
/*
 * 除去输入字符串中的空格
 */
void stripspaces(char *s) {
    unsigned i = 0;
    size_t len = strlen(s);
    for(i = 0; i < len; i++)
        if(!isspace(s[i]))
            break;
    if(i < len) {
        memmove(s, s + i, len - i);
        s[len - i] = 0;
    }
    for(i = strlen(s) - 1; strlen(s) > 0 && i >= 0; i--)
        if(isspace(s[i]))
            s[i] = 0;
        else
            break;
}
/*
 * 该函数找到输入字符串中第一个":"字符，以此为分界线把输入字符串分割为两部分，并且去掉两部分前后的空格
 */
bool split(const char *s, char **key, char **value) {
    const char *valueptr = strchr(s, ':');
    if(valueptr == NULL)
        return false;
    *key = (char *) malloc(valueptr - s + 1);
    memmove(*key, s, valueptr - s);
    (*key)[valueptr - s] = 0;
#ifdef _WIN32
    *value = _strdup(valueptr + 1);
#else
    *value = strdup(valueptr + 1);
#endif
    stripspaces(*key);
    stripspaces(*value);
    return true;
}

#ifdef _WIN32
int getline(char **line, size_t *size, FILE *fp) {
	int len = -1;
	char c;
	while(true) {
		if(fread(&c, 1, 1, fp) <= 0)
			return len;
		len++;
		if((unsigned) len >= *size) {
			*size += 256;
			*line = (char *) realloc(*line, *size);
		}
		(*line)[len] = c;
		(*line)[len + 1] = 0;
	}
	return -1;
}
#endif

ConfigFile::ConfigFile(const char* filename) {
#ifndef _WIN32
    FILE *fp = fopen(filename, "r");
#else
	FILE *fp;
	fopen_s(&fp, filename, "r");
#endif
	if(fp == NULL)
		throw "Cannot open gVirtuS config file.";
    char *line = NULL;
    size_t size = 0;
    while(getline(&line, &size, fp) >= 0) {//从文件fp中一行一行的读出数据放到line中
        eatcomments(line);//讲linez中第一个'#'字符修改为'\0'
        stripspaces(line);//把line中的空格去掉
        if(strlen(line) == 0)
            continue;
        char *key, *value;
        if(!split(line, &key, &value))
            throw "Invalid entry in config file.";
        mValues.insert(make_pair(string(key), string(value)));

        //Sandy 2016.04.14
        //用来从配置文件中读出后端GPU的数目，并且复制给静态数据成员backend_gpu
        if( (strcmp(key,"gvirtus_device_count")) == 0)
        {
        	backend_gpu = atoi(value);
        }
        if( (strcmp(key,"communicator")) == 0)	//条件如果成立，表明value的内容为“afunix:///tmp/gvirtus:0666”类似的字符串，IP地址包含在内了 Sandy
        {
        	mIPaddrs.push_back(value);//Sandy
        }

        free(key);
        free(value);
    }
    free(line);
    fclose(fp);
}

ConfigFile::~ConfigFile() {

}

const string tolower(const std::string &s) {
    string l;
    stringstream ss;

    for(unsigned i = 0; i < s.length(); i++)
        ss << (char)(tolower(s[i]));
    return ss.str();
}

bool ConfigFile::HasKey(const std::string& key) const {
    map<string,string>::const_iterator i = mValues.find(tolower(key));
    return i != mValues.end();
}
const string ConfigFile::Get(const std::string& key) const {
    map<string,string>::const_iterator i = mValues.find(tolower(key));
    if(i == mValues.end())
        throw "Key not found.";
    return i->second;
}
/*Sandy 2016.03.11 */
const string ConfigFile::Get_IPs(const std::string& key,const int index)
{
    map<string,string>::iterator i = mValues.find(tolower(key));//mValues中的内容应该是配置文件中的所有的<communicator,tcp://localhost:port>形式
    if(i == mValues.end())
    {
        throw "Key not found.";
    }
    return mIPaddrs[index];
}
/* Sandy 2016.04.14  */
int ConfigFile::Get_gpuNum(void)
{
	return backend_gpu;
}

void ConfigFile::Dump() {
    for(map<string,string>::iterator i = mValues.begin(); i != mValues.end();
            i++)
        cout << "ConfigFile[" << i->first << "]: " << i->second << endl;
}
