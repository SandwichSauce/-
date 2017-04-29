//
//  main.cpp
//  分酒
//
//  Created by 祝歆韵 on 2017/4/28.
//  Copyright © 2017年 祝歆韵. All rights reserved.
//
#include <iostream>
#include <map>
#include <queue>
#include <iomanip>
using namespace std;
int cnt = 0; //定义变量cnt用于计算用了多少步
//定义每个状态的结构
struct SNode
{
    int state[3]; //状态值
    int id; //状态的id（产生这个状态的序号）
    int pre; //当前状态的前驱状态id(由于是广度搜索，前驱即广度搜索树中的父节点)
    bool operator < (const SNode & s) const //节点存放时的比较函数
    {
        return id < s.id;
    }
    bool operator == (const SNode &s) const //判断节点是否相等，当存放在map中时需要判断
    {
        return (state[0] == s.state[0] ) && (state[1] == s.state[1] ) && (state[2] == s.state[2]);
    }
};

SNode curState; //当前节点的状态
map<int, SNode> save; //保存中间节点的数据结构<节点id, 节点结构>
int global_id = 0; //节点id通过global_id逐渐增加
int pour(int i, int j) //判断状态转移（即能否倒酒从一个容器到另一个）
{
    if(i == j)
        return 0;
    if(curState.state[i] <= 0)
        return 0;
    int empty = 0;
    if(i == 0) //如果从0号瓶倒出（容量为12斤）
    {
        if(j == 1) //如果倒入1号瓶（容量为10斤）
        {
            empty = 5 - curState.state[j];
            if(curState.state[i] < empty)
                return curState.state[i];
            else
                return empty;
        }
        else if(j == 2) //如果倒入2号瓶（容量为6斤）
        {
            empty = 3 - curState.state[j];
            if(curState.state[i] < empty)
                return curState.state[i];
            else
                return empty;
        }
    }
    
    else if(i == 1) //如果从1号瓶倒出（容量为10斤）
    {
        if(j == 0) //如果倒入0号瓶（容量为12斤）
        {
            empty = 8 - curState.state[j];
            if(curState.state[i] < empty)
                return curState.state[i];
            else
                return empty;
        }
        else if(j == 2) //如果倒入2号瓶（容量为6斤）
        {
            empty = 3 - curState.state[j];
            if(curState.state[i] < empty)
                return curState.state[i];
            else
                return empty;
        }
    }
    
    else if(i == 2 ) //如果从2号瓶倒出（容量为6斤）
    {
        if(j == 0) //如果倒入0号瓶（容量为12斤）
        {
            empty = 8 - curState.state[j];
            if(curState.state[i] < empty)
                return curState.state[i];
            else
                return empty;
        }
        else if(j == 1) //如果倒入1号瓶（容量为10斤）
        {
            empty = 5 - curState.state[j];
            if(curState.state[i] < empty)
                return curState.state[i];
            else
                return empty;
        }
    }
    return 0;
}

void move(int i, int j, int n) //从i号瓶中倒出n斤到j号瓶中
{
    curState.state[i] -= n;
    curState.state[j] += n;
}

bool hasExist() //判断是否有重复的中间状态
{
    map<int, SNode> :: iterator it = save.begin();
    while(it != save.end())
    {
        
        if(it -> second == curState)
            return true;
        it++;
    }
    return false;
}

bool isTarget(SNode node) //到达目的状态(4,4,0)
{
    if( node.state[0] == 4 && node.state[1] == 4 && node.state[2] == 0 )
        return true;
    else
        return false;
}

void show(SNode cur) //根据每个节点的前驱节点pre回溯打印出整个搜索路径
{
    map<int, SNode> :: iterator it;
    
    if(cur.pre == -1) //到了根节点
    {
        cout << setw(4) << cur.state[0] << setw(4) << cur.state[1] << setw(4) << cur.state[2] << endl;
        return;
    }
    else
    {
        it = save.lower_bound(cur.pre);
        curState = it -> second;
        show(curState);
        cout << setw(4) << cur.state[0] << setw(4) << cur.state[1] << setw(4) << cur.state[2] << endl;
        cnt++;
        
    }
}

int main()
{
    int i = 0, j = 0;
    queue<SNode> queue; //广度搜索时的队列
    SNode initState; //初始化状态
    initState.id = global_id++;
    initState.pre = -1;
    initState.state[0] = 8;
    initState.state[1] = 0;
    initState.state[2] = 0;
    curState = initState;
    int n = 0;
    int pre = -1;
    //初始化状态如对列并作为中间状态保存在save中
    queue.push(initState);
    pair<int,SNode> p(initState.id, initState);
    save.insert(p);
    while(!queue.empty())
    {
        curState = queue.front();
        queue.pop();
        pre = curState.id;
        for(i = 0; i < 3; i++)
            for(j = 0; j < 3; j++)
            {
                if((n = pour(i,j)) > 0)
                {
                    move(i, j, n);//通过pour和move由当前状态转移到下一个状态
                    if(!hasExist())//判断新状态是否已经存在
                    {
                        curState.pre = pre;
                        curState.id = global_id++;
                        if(isTarget(curState))
                        {
                            //如果到达了终态，打印出搜索路径
                            pair<int, SNode> p(curState.id, curState);
                            save.insert(p);
                            show(curState);
                            cout << "该过程总共用了" << cnt << "步。" <<endl;
                            return 0;
                        }
                        //新状态如对并作为中间状态保存在save中
                        queue.push(curState);
                        pair<int,SNode> p(curState.id, curState);
                        save.insert(p);
                    }
                    //继续广度搜索到下一个状态
                    move(j, i, n);
                }
            }
    }
    cout << "该过程总共用了" << cnt << "步。" <<endl;
    return 0;
}



