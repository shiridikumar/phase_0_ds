#include "global.h"

Cursor::Cursor(string tableName, int pageIndex)
{
    // //<<"cursor function"<<endl;
    logger.log("Cursor::Cursor");
    this->page = bufferManager.getPage(tableName, pageIndex);
    this->pagePointer = 0;
    this->tableName = tableName;
    this->pageIndex = pageIndex;
}

/**
 * @brief This function reads the next row from the page. The index of the
 * current row read from the page is indicated by the pagePointer(points to row
 * in page the cursor is pointing to).
 *
 * @return vector<int>
 */
vector<int> Cursor::getNext()
{
    if (parsedQuery.ismatrix)
    {
        logger.log("Cursor::geNext");
        vector<int> sep = tableCatalogue.getTable(this->tableName)->sep;
        vector<int> result;
        this->page.getRow(this->pagePointer, sep, result);

        int limit = (int)tableCatalogue.getTable(this->tableName)->columnCount;

        if (parsedQuery.queryType == PRINT)
            limit = min(20, limit);

        while (result.size() < limit && this->pagePointer < tableCatalogue.getTable(this->tableName)->rowCount)
        {
            int temp = this->pagePointer;
            tableCatalogue.getTable(this->tableName)->getNextPage(this);
            if (!(pagePointer))
            {
                this->pagePointer = temp;
                this->page.getRow(this->pagePointer, sep, result);
            }
            else
            {
                break;
            }
            if (this->pageIndex == tableCatalogue.getTable(this->tableName)->blockCount - 1)
            {
                break;
            }
        }
        this->pagePointer++;
        return result;
    }
    else
    {
        logger.log("Cursor::geNext");
        vector<int> result = this->page.getRow(this->pagePointer);
        this->pagePointer++;
        if (result.empty())
        {
            tableCatalogue.getTable(this->tableName)->getNextPage(this);
            if (!this->pagePointer)
            {
                result = this->page.getRow(this->pagePointer);
                this->pagePointer++;
            }
        }
        return result;
    }
}

void Cursor::transposeLine(int row)
{
    string tname = this->tableName;
    vector<int> res;
    int ind = 0;
    ofstream fout2(parsedQuery.crossTransSecondMatrix, ios::out);
    vector<int> sep = tableCatalogue.getTable(tname)->sep;
    for (int row = 0; row < tableCatalogue.getTable(tname)->rowCount; row++)
    {
        int pageindex = ((row * tableCatalogue.getTable(tname)->columnCount)) / (tableCatalogue.getTable(tname)->maxElementsperblock);
        Table table2 = *(tableCatalogue.getTable(parsedQuery.crossTransSecondMatrix));
        int done = 0;
        int start = (row == 0) ? 0 : sep[row - 1];
        int st = (row == 0) ? 0 : sep[row - 1];
        int end = min(start + tableCatalogue.getTable(tname)->columnCount, tableCatalogue.getTable(tname)->maxElementsperblock);
        pageindex = ((row * tableCatalogue.getTable(tname)->columnCount)) / (tableCatalogue.getTable(tname)->maxElementsperblock);
        Cursor cursor2(table2.tableName, 0);
        while (done != tableCatalogue.getTable(tname)->columnCount)
        {
            while (this->pageIndex != pageindex)
            {
                ofstream fout1(this->page.pageName, ios::trunc);
                for (int i = 0; i < res.size(); i++)
                {
                    if (res[i] == sep[ind])
                    {
                        fout1 << endl;
                    }
                    fout1 << res[i] << " ";
                }
                fout1.close();

                res.clear();
                tableCatalogue.getTable(tname)->getNextPage(this);
            }
            cursor2.getnextline(row, done, done + (end - start), pageindex, res);
            done += (end - start);
            start = 0;
            end = min(tableCatalogue.getTable(tname)->columnCount - done, tableCatalogue.getTable(tname)->maxElementsperblock - done);
            if (pageindex < tableCatalogue.getTable(tname)->blockCount - 1)
            {
                pageindex += 1;
            }
            else
            {
                break;
            }
        }
    }
    ofstream fout1(this->page.pageName, ios::trunc);
    for (int i = 0; i < res.size(); i++)
    {
        if (res[i] == sep[ind])
        {
            fout1 << endl;
        }
        fout1 << res[i] << " ";
    }
    fout1.close();
}

vector<int> Cursor::getnextline(int j, int start, int end, int pageindex, vector<int> &res)
{
    string tname = this->tableName;
    vector<int> ans(tableCatalogue.getTable(tname)->rowCount, 0);
    int ind;
    vector<int> sep = tableCatalogue.getTable(tname)->sep;
    for (int i = start; i < end; i++)
    {
        pageindex = ((i * tableCatalogue.getTable(tname)->columnCount)) / (tableCatalogue.getTable(tname)->maxElementsperblock);
        if (i == 0)
        {
            ind = 0;
        }
        else
        {
            ind = sep[i - 1];
        }
        int additional = j / tableCatalogue.getTable(tname)->maxElementsperblock;
        int rem = (ind + j) % tableCatalogue.getTable(tname)->maxElementsperblock;
        pageindex += additional;

        while (this->pageIndex != pageindex)
        {
            tableCatalogue.getTable(tname)->getNextPage(this);
        }
        tableCatalogue.getTable(tname)->sep;
        res.push_back(this->page.getRowElement(i, rem, tableCatalogue.getTable(tname)->sep));
    }
    return ans;
}

/**
 * @brief Function that loads Page indicated by pageIndex. Now the cursor starts
 * reading from the new page.
 *
 * @param pageIndex
 */
void Cursor::nextPage(int pageIndex)
{
    logger.log("Cursor::nextPage");
    this->page = bufferManager.getPage(this->tableName, pageIndex);
    this->pageIndex = pageIndex;
    this->pagePointer = 0;
}