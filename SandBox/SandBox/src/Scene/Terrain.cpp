#include "Terrain.h"
#include <iostream>
#include <algorithm>


Terrain::Terrain()
	:terrain(std::vector<std::vector<glm::vec3>>()), current_row(int()), current_colum(int()), rows(int()), columns(int())
{};
Terrain::~Terrain()
{};

void Terrain::Init(std::vector<float> pos)
{
    std::vector<float> xtemp = std::vector<float>();
    std::vector<float> ztemp = std::vector<float>();
    
    if (pos.size() < 3)
    {
        std::cout << "abort terrain init!" << std::endl;
    }
    else
    {
        for (size_t i = 0; i < pos.size(); i += 3)
        {
            xtemp.push_back(pos[i]);
            ztemp.push_back(pos[i + 2]);
        }

        std::sort(xtemp.begin(), xtemp.end());
        xtemp.erase(std::unique(xtemp.begin(), xtemp.end()), xtemp.end());

        std::sort(ztemp.begin(), ztemp.end());
        ztemp.erase(std::unique(ztemp.begin(), ztemp.end()), ztemp.end());

        std::vector<glm::vec3> tempRow = std::vector<glm::vec3>();

        for (size_t i = 0; i < ztemp.size(); i ++)
        {
            for (size_t j = 0; j < xtemp.size(); j++)
            {
                for (size_t k = 0; k < pos.size(); k += 3)
                {
                    if ((xtemp[j] == pos[k]) && (ztemp[i] == pos[k + 2]))
                    {
                        tempRow.push_back(glm::vec3(pos[k], pos[k + 1], pos[k + 2]));
                        break;
                    }
                }
            }
            terrain.push_back(tempRow);
            tempRow = std::vector<glm::vec3>();
        }

        rows = xtemp.size() - 2;
        columns = ztemp.size() - 2;
        /*
              0          1          2          3          4          5          columns size = 6
         0          1          2          3          4          5          6    positions size = 7 -> columns = positions size - 2;
       0 + -------- + -------- + -------- + -------- + -------- + -------- +
         |          |          |          |          |          |          |
    0    |          |          |          |          |          |          |
         |          |          |          |          |          |          |
       1 + -------- + -------- + -------- + -------- + -------- + -------- +
         |          |          |          |          |          |          |
    1    |          |          |          |          |          |          |
         |          |          |          |          |          |          |
       2 + -------- + -------- + -------- + -------- + -------- + -------- +
         |          |          |          |          |          |          |
    2    |          |          |          |          |          |          |
         |          |          |          |          |          |          |
       3 + -------- + -------- + -------- + -------- + -------- + -------- +

        */


        /*
            #include <chrono>
            auto start = std::chrono::high_resolution_clock::now();
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            std::cout << "time: " << duration.count() << std::endl;
        */
        /* tested only the for for for if loop (vector push_back without reserve: time < 0.46 seconds)*/
        // std::unordered_map<int, std::unordered_map<int, glm::vec3>>
        // time = 466145 microseconds test 1
        // time = 466631 microseconds test 2
        // time = 467786 microseconds test 3
        // time = 465923 microseconds test 4
        // time = 469772 microseconds test 5
        // time = 529561 microseconds test 6

        // std::vector<std::vector<glm::vec3>>
        // vector reserve and emplace_back: time = 458864 microseconds, test 1
        // vector reserve and emplace_back: time = 458838 microseconds, test 2
        // vector reserve and emplace_back: time = 456937 microseconds, test 3
        // vector reserve and emplace_back: time = 458566 microseconds, test 4

        // vector emplace_back without reserve: time = 564088 microseconds, test 1
        // vector emplace_back without reserve: time = 456553 microseconds, test 2
        // vector emplace_back without reserve: time = 574574 microseconds, test 3

        // vector push_back without reserve: time = 456990 microseconds, test 1
        // vector puch_back without reserve: time = 457526 microseconds, test 2
        // vector puch_back without reserve: time = 453743 microseconds, test 3
        // vector puch_back without reserve: time = 452133 microseconds, test 4
        // vector puch_back without reserve: time = 453969 microseconds, test 5

        // vector push_back and reserve: time = 453357 microseconds, test 1
        // vector push_back and reserve: time = 456838 microseconds, test 2
        // vector push_back and reserve: time = 455496 microseconds, test 3
        // vector puch_back and reserve: time = 513354 microseconds, test 4
        // vector puch_back and reserve: time = 456335 microseconds, test 5
    }  
}

bool Terrain::FindStartPosition(glm::vec3 pos)
{
    bool status = false;
    for (size_t i = 0; i < terrain.size(); i ++)
    {
        if(terrain[i][0].z >= pos.z)
        {
            current_row = i;
            status = true;
            break;
        }
        else
            status = false;
    }
    if (status)
    {
        for (size_t i = 0; i < terrain[current_row].size(); i ++)
        {
            if (terrain[current_row][i].x >= pos.x)
            {
                status = true;
                current_colum = i;
                break;
            }
            else
                status = false;
        }
    }

    return status;
}

void Terrain::PrintPlane()
{
    std::cout << "################# TopLeft     ################" << std::endl;
    std::cout << "x: " << getTopLeft().x << ", y: " << getTopLeft().y << ", z: " << getTopLeft().z << "\n" << std::endl;
    std::cout << "################# TopRight    ################" << std::endl;
    std::cout << "x: " << getTopRight().x << ", y: " << getTopRight().y << ", z: " << getTopRight().z << "\n" << std::endl;
    std::cout << "################# BottomLeft  ################" << std::endl;
    std::cout << "x: " << getBottomLeft().x << ", y: " << getBottomLeft().y << ", z: " << getBottomLeft().z << "\n" << std::endl;
    std::cout << "################# BottomRight ################" << std::endl;
    std::cout << "x: " << getBottomRight().x << ", y: " << getBottomRight().y << ", z: " << getBottomRight().z << "\n" << "\n" << std::endl;
}

// can return float y pos
float Terrain::OnUpdate(glm::vec3& pos)
{
    if (pos.x >= getTopLeft().x && pos.x <= getTopRight().x && pos.z >= getTopLeft().z && pos.z <= getBottomLeft().z)
    {
        return getCurrentPosition(&pos);
    }
    else
    {
        while (pos.x < getTopLeft().x)
        {
            current_colum--;
            if (current_colum < 0)
            {
                current_colum = 0;
                pos.x = getTopLeft().x;
                //PrintPlane();
                break;
            }
        }

        while (pos.x > getTopRight().x)
        {
            current_colum++;
            if (current_colum > columns)
            {
                current_colum = columns;
                pos.x = getTopRight().x;
                //PrintPlane();
                break;
            }
        }

        while (pos.z < getTopLeft().z)
        {
            current_row--;
            if (current_row < 0)
            {
                current_row = 0;
                pos.z = getTopLeft().z;
                //PrintPlane();
                break;
            }
        }

        while (pos.z > getBottomLeft().z)
        {
            current_row++;
            if (current_row > rows)
            {
                current_row = rows;
                pos.z = getBottomLeft().z;
                //PrintPlane();
                break;
            }
        }

        return getCurrentPosition(&pos);
    }
}

float baryCentric(const glm::vec3* p1, const glm::vec3* p2, const glm::vec3* p3, const glm::vec3* pos)
{
    float det = (p2->z - p3->z) * (p1->x - p3->x) + (p3->x - p2->x) * (p1->z - p3->z);
    float l1 = ((p2->z - p3->z) * (pos->x - p3->x) + (p3->x - p2->x) * (pos->z - p3->z))/det;
    float l2 = ((p3->z - p1->z) * (pos->x - p3->x) + (p1->x - p3->x) * (pos->z - p3->z))/det;
    float l3 = 1.0f - l1 - l2;

    return l1 * p1->y + l2 * p2->y + l3 * p3->y;
}

float Terrain::getCurrentPosition(const glm::vec3* pos)
{
    if((getTopLeft().x - pos->x) <= ((getTopLeft().z - getBottomLeft().z) - (getTopLeft().z - pos->z)))
    {
        return baryCentric(&getTopLeft(), &getBottomLeft(), &getTopRight(), pos);
    }
    else
    {
        return baryCentric(&getBottomLeft(), &getBottomRight(), &getTopRight(), pos);
    }
        
}