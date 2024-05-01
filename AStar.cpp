#include "RouteCreator.h"



using namespace std;

template <typename T> void remove(vector<T>& v, size_t index) {
    v.erase(v.begin() + index);
};

void RouteCreator::create(Point& start, Point& end, vector<Point*>* out) 
{
    int n = 100;
    int m = 70;

    int dx[] = { -1, 0, 1, 0 };
    int dy[] = { 0, 1, 0, -1 };

    vector<Node*> open_list;
    vector<Node*> closed_list;

    Node* start_node = new Node(start.getX(), start.getY());
    start_node->G = 0;
    start_node->H = 0;
    start_node->F = 0;

    open_list.push_back(start_node);

    while (!open_list.empty()) 
    {

        //prioriy queue 구현부
        Node* current_node = open_list[0];
        int current_index = 0;
        for (int i = 0; i < open_list.size(); i++) 
        {

            if (open_list[i]->F < current_node->F) 
            {
                current_node = open_list[i];
                current_index = i;

            }
        }

        remove(open_list, current_index);
        closed_list.push_back(current_node);

        //목적지에 도착했을 때
        if (current_node->x == end.getX() && current_node->y == end.getY()) 
        {


            Node* current = current_node;
            while (current != NULL) {
                Point* p = new Point(current->x, current->y);
                out->push_back(p);
                Node* tmp = current;
                current = current->getParent();
                delete tmp;
            }
            out->pop_back();
            reverse(out->begin(), out->end());
            

            open_list.clear();
            closed_list.clear();
            vector<Node*>().swap(open_list);
            vector<Node*>().swap(closed_list);

            return;
        }

        //인접 정점 수집
        for (int i = 0; i < 4; i++) 
        {

            int new_x = current_node->x + dx[i];
            int new_y = current_node->y + dy[i];


            //유효범위 이탈
            if (new_x < 0 || new_x >= n || new_y < 0 || new_y >= m) {
                continue;
            }

            //진행불가구역 스킵
            /* m의 위치가 목적지랑 동일하기 때문에 경로를 진입할 수 없음.
            if ((*map)[new_x][new_y] == '1' || (*map)[new_x][new_y] == 'm') {
                continue;
            }
            */
            
            if ((*map)[new_x][new_y] == '1') {
                continue;
            }
            
            //인접 노드 객체 생성
            Node* child = new Node(current_node, new_x, new_y);


            //탐색 종료된 영역이면 패스
            for (int i = 0; i < closed_list.size(); i++) {
                Node* closed = closed_list[i];

                if (*child == *closed) {
                    delete child;
                    child = NULL;
                    break;
                }
            }

            if (child == NULL) {
                continue;
            }

            //새로운 영역 휴리스틱값 계산
            child->G = current_node->G + 1;
            child->H = pow((child->x - end.getX()), 2) + pow((child->y - end.getY()), 2);
            child->F = child->G + child->H;


            //이미 탐색예정목록에 추가된 노드일 경우 다른쪽에서 접근하는 경우가 더 빠를 경우 탐색예정목록에 미추가
            for (const Node* node : open_list) {
                if (*child == *node) 
                {
                    if (child->G >= node->G) 
                    {
                        delete child;
                        child = NULL;
                        break;
                        
                    }
                    
                }
            }
            if (child == NULL) {
                continue;
            }

            open_list.push_back(child);
        }
    }
}


