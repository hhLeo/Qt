#ifndef DELAUNAY_H
#define DELAUNAY_H

#include "dots.h"
#include <QDebug>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <ctime>
#include <list>
#include <set>
#include <queue>
#include <cmath>
#include <vector>
#include <bitset>
#include <functional>

using namespace std;

class Delaunay
{
public:
    //++++++++++++++++++
    typedef long long LL;
    typedef double ld;

    const static int MAX=400000+10;//static
    const static int NUM=20;

    int n;
    double use_time;
    Delaunay(){use_time = 0;}

    class Line
    {
    public:
        int x;
        int y;
    }lines[MAX];

    struct point
    {
        LL x,y;
        int num;
        point(){}
        point(LL a,LL b)
        {
            x=a;
            y=b;
        }
    }d[MAX];

    friend int operator < (const point& a,const point& b)//friend
    {
        if(a.x!=b.x)return a.x<b.x;
        else return a.y<b.y;
    }

    friend point operator - (const point& a,const point& b)
    {
        return point(a.x-b.x,a.y-b.y);
    }

    LL chaji(const point& s,const point& a,const point& b)
    {
        return (a.x-s.x)*(b.y-s.y)-(a.y-s.y)*(b.x-s.x);
    }

    LL dist(const point& a,const point& b)
    {
        return (a.x-b.x)*(a.x-b.x)+(b.y-a.y)*(b.y-a.y);
    }

    struct point3
    {
        LL x,y,z;
        point3(){}
        point3(LL a,LL b,LL c)
        {
            x=a;
            y=b;
            z=c;
        }
        point3(point a)
        {
            x=a.x;
            y=a.y;
            z=x*x+y*y;
        }
    };

    friend point3 operator - (const point3 a,const point3& b)
    {
        return point3(a.x-b.x,a.y-b.y,a.z-b.z);
    }

    point3 chaji(const point3& a,const point3& b)
    {
        return point3(a.y*b.z-a.z*b.y,-a.x*b.z+a.z*b.x,a.x*b.y-a.y*b.x);
    }

    LL dianji(const point3& a,const point3& b)
    {
        return a.x*b.x+a.y*b.y+a.z*b.z;
    }

    LL in_circle(point a,point b,point c,point d)
    {
        if(chaji(a,b,c)<0)
            swap(b,c);
        point3 aa(a),bb(b),cc(c),dd(d);
        bb=bb-aa;cc=cc-aa;dd=dd-aa;
        point3 f=chaji(bb,cc);
        return dianji(dd,f);
    }

    struct Edge
    {
        int t;
        list<Edge>::iterator c;
        Edge(){}
        Edge(int v)
        {
            t=v;
        }
    };
    list<Edge> ne[MAX];

    void add(int a,int b)
    {
        ne[a].push_front(b);
        ne[b].push_front(a);
        ne[a].begin()->c=ne[b].begin();
        ne[b].begin()->c=ne[a].begin();
    }

    int sign(LL a)
    {
        return a>0?1:(a==0?0:-1);
    }

    int cross(const point& a,const point& b,const point& c,const point& d)
    {
        return sign(chaji(a,c,b))*sign(chaji(a,b,d))>0 && sign(chaji(c,a,d))*sign(chaji(c,d,b))>0;
    }

    void work(int l,int r)
    {
        int i,j,nowl=l,nowr=r;
        list<Edge>::iterator it;
        if(l+2>=r)
        {
            for(i=l;i<=r;++i)
                for(j=i+1;j<=r;++j)
                    add(i,j);
            return;
        }
        int mid=(l+r)/2;
        work(l,mid);work(mid+1,r);
        int flag=1;
        for(;flag;)
        {
            flag=0;
            point ll=d[nowl],rr=d[nowr];
            for(it=ne[nowl].begin();it!=ne[nowl].end();++it)
            {
                point t=d[it->t];
                LL s=chaji(rr,ll,t);
                if(s>0 || ( s==0 && dist(rr,t)<dist(rr,ll) ) )
                {
                    nowl=it->t;
                    flag=1;
                    break;
                }
            }
            if(flag)
                continue;
            for(it=ne[nowr].begin();it!=ne[nowr].end();++it)
            {
                point t=d[it->t];
                LL s=chaji(ll,rr,t);
                if(s<0 || (s==0 && dist(ll,rr)>dist(ll,t) ) )
                {
                    nowr=it->t;
                    flag=1;
                    break;
                }
            }
        }
        add(nowl,nowr);
        for(;1;)
        {
            flag=0;
            int best=0,dir=0;
            point ll=d[nowl],rr=d[nowr];
            for(it=ne[nowl].begin();it!=ne[nowl].end();++it)
                if(chaji(ll,rr,d[it->t])>0 && ( best==0 || in_circle(ll,rr,d[best],d[it->t])<0 ) )
                    best=it->t,dir=-1;
            for(it=ne[nowr].begin();it!=ne[nowr].end();++it)
                if(chaji(rr,d[it->t],ll)>0 && ( best==0 || in_circle(ll,rr,d[best],d[it->t])<0 ) )
                    best=it->t,dir=1;
            if(!best)break;
            if(dir==-1)
            {
                for(it=ne[nowl].begin();it!=ne[nowl].end();)
                    if(cross(ll,d[it->t],rr,d[best]))
                    {
                        list<Edge>::iterator ij=it;
                        ++ij;
                        ne[it->t].erase(it->c);
                        ne[nowl].erase(it);
                        it=ij;
                    }
                    else ++it;
                nowl=best;
            }
            else if(dir==1)
            {
                for(it=ne[nowr].begin();it!=ne[nowr].end();)
                    if(cross(rr,d[it->t],ll,d[best]))
                    {
                        list<Edge>::iterator ij=it;
                        ++ij;
                        ne[it->t].erase(it->c);
                        ne[nowl].erase(it);
                        it=ij;
                    }
                    else ++it;
                nowr=best;
            }
            add(nowl,nowr);
        }
    }

    struct MstEdge
    {
        int x,y;
        LL w;
    }e[MAX];
    int m;

    friend int operator < (const MstEdge& a,const MstEdge& b)
    {
        return a.w<b.w;
    }

    int fa[MAX];

    int findfather(int a)
    {
        return fa[a]==a?a:fa[a]=findfather(fa[a]);
    }

    int hash[MAX],p[MAX/4][NUM],deep[MAX],place[MAX];
    LL dd[MAX/4][NUM];

    vector<int> ne2[MAX];
    queue<int> q;

    LL getans(int u,int v)
    {
        if(deep[u]<deep[v])
            swap(u,v);
        LL ans=0;
        int s=NUM-1;
        while(deep[u]>deep[v])
        {
            while(s && deep[p[u][s]]<deep[v])--s;
            ans=max(dd[u][s],ans);
            u=p[u][s];
        }
        s=NUM-1;
        while(u!=v)
        {
            while(s && p[u][s]==p[v][s])--s;
            ans=max(dd[u][s],ans);
            ans=max(dd[v][s],ans);
            u=p[u][s];
            v=p[v][s];
        }
        return ans;
    }

    virtual vector <Line> getTreeLine(){
        vector <Line> l;
        l.clear();
        int i;
        for(i = 1; i < n; i++){
            l.push_back(lines[i]);
        }
        return l;
    }

    virtual vector <Line> getDelaunay(){
        vector <Line> l;
        l.clear();
        int i;
        for(i = 1; i <= m; i++){
            Line b;
            b.x = d[e[i].x].num;
            b.y = d[e[i].y].num;
            l.push_back(b);
        }
        qDebug()<<"L size"<<l.size();
        return l;
    }

    virtual int run(vector <Dots::Coordinate> dots)
    {
        time_t t = clock();
        srand(time(0));

        m = 0;
        int i;
        n = dots.size();
        for(i = 1; i <= n; i++)
        {
            d[i].x = dots[i-1].x;
            d[i].y = dots[i-1].y;
            d[i].num = i;
            ne[i].clear();
        }
        sort(d+1,d+n+1);
        for(i=1;i<=n;++i)
            place[d[i].num]=i;
        work(1,n);
        for(i=1;i<=n;++i)
            for(list<Edge>::iterator it=ne[i].begin();it!=ne[i].end();++it)
            {
                if(it->t<i)continue;
                ++m;
                e[m].x=i;
                e[m].y=it->t;
                e[m].w=dist(d[e[m].x],d[e[m].y]);
            }
        sort(e+1,e+m+1);
        for(i=1;i<=n;++i)
            fa[i]=i;
        int count = 1;
        for(i=1;i<=m;++i)
            if(findfather(e[i].x)!=findfather(e[i].y))
            {
                fa[findfather(e[i].x)]=findfather(e[i].y);
                lines[count].x = d[e[i].x].num;
                lines[count++].y = d[e[i].y].num;
            }

        use_time = (clock()-t)*1.0/CLOCKS_PER_SEC*1000;
        qDebug()<<(clock()-t)*1.0/CLOCKS_PER_SEC*1000<<"ms";
        return 0;
    }

    //++++++++++++++++++
};

#endif // DELAUNAY_H
