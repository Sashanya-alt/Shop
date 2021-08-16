/*************************************************************************
*                                                                        *
*                  Информатика и вычислительная техника                  *
*                                                                        *
**************************************************************************
*                                                                        *
*      Project type:  Win32 Console Application                          *
*      Project name:  Курсовой проект		                             *
*      File name   :  Курсовая работа.cpp		                         *
*      Language    :  cpp MSVS 2019                                      *
*      Programmers :  M30-207Б-19                                        *
*                     Давыдова Александра Андреевна                      *
*      Modified by :  20.12.2020                                         *
*      Created     :  03.11.2020                                         *
*                                                                        *
*************************************************************************/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>		//Библиотека ввода-вывода
#include <locale.h>		//Для поддержки локализации (национальных особенностей) при работе с датами, стоками, символами и т.п
#include <string.h>		//Работа с Си-строками (массивами символов)
#include <time.h>		//Для работы с системной датой и временем используется
#include <stdlib.h>
#include <math.h>		//Математическая библиотека
using namespace std;

/*************************************************************************
*               Г Л О Б А Л Ь Н Ы Е    К О Н С Т А Н Т Ы                 *
*************************************************************************/

const int MAXSUP = 30;							//максимальная дина названия производителя
const int MAXPR = 40;							//максимальная длина названия продукта
const char* SUPBASE = "supplier base.bin";		//файл с информации о поставщиках
const char* PRBASE = "products.bin";			//файл с информации о товарах
const char* COPY = "copy of file.bin";			//файл, необходимый для изменения кол-ва товаров
const char* CART = "cart with products.bin";					//файл с продаваемыми товарами
const int LEN = 90;								//максимальная длина строки в файле

struct product_inf {							// информация о продукте
	int ID_P;									//код товара
	char pr_name[MAXPR + 1];					//название товара
	int quantity;								//количество товара
	int price;									//цена товара
	int ID_S;									//код производителя
	char sup_name[MAXSUP + 1];					//имя производителя
	long place;									//положение необходимого товара в файле
};


/*************************************************************************
*                  П Р О Т О Т И П Ы    Ф У Н К Ц И Й                    *
*************************************************************************/

int SupplyProduct	(void);										//поставка товара в магазин
int Sale			(void);										//продажа товара
int PrintPR			(void);										//печать данных из файла с товарами
int PrintSUP		(void);										//печать данных из файла с поставщиками
int Existence		(const char(*key), int ID);					//проверка существования товара или поставщика
int Writer			(const char(*key), int ID);					//запись нового продукта или производителя
int CheckQuantity	(int ID, int quantity);						//проверяет наличие необходимого кол-ва товара
int ChangeQuantity	(const char(*key), int ID, int quantity);	//увеличивает или уменьшает количество товара
int Bill			(void);										//формирование чека
tm today			(void);										//определение текущей даты
int getint			(int &digit);								//получение числа с клавиатуры
int CopyChange		(const char(*key), int ID, int quantity);	//печать данных из одного файла в другой
int CopyTocart		(int ID, int quantity, int code);			//печать определенной строки из одного файла в другой
void PrintMessage	(int error_code);							//печать результата о работе программы

/*************************************************************************
*                 О С Н О В Н А Я    П Р О Г Р А М М А                   *
*************************************************************************/

int main()
{
	setlocale(LC_ALL, "RUS");			//подключение русского языка
	
	int code = 0;						//код операции
	int error;							//код ошибки
	FILE* p;							//продукция
	FILE* s;							//поставщики

	p = fopen(PRBASE, "rb");			//открытие файла с продуктами для чтения

	//проверка наличия файла
	if (!p)								//файла нет в директории проекта
	{
		PrintMessage(-1);
		return -1;
	}//end if

	s = fopen(SUPBASE, "rb");			//открытие файла с поставщиками для чтения

	//проверка наличия файла
	if (!s)								//файла нет в директории проекта
	{
		PrintMessage(-1);
		return -1;
	}//end if

	//закрыть файлы
	fclose(p);
	fclose(s);

	//печать возможных команд
	printf("Нажмите цифру, которая соответствует необходимой операции \n"
		"1 - приема товара\n"
		"2 - продажи товара\n"
		"3 - просмотр всех товаров или поставщиков\n");

	 error = getint(code);//получение номера операции с клавиатуры

	 while (error == INT_MAX)		//пока введенные данные не корректны
	 {
		 printf("Полученные данные некорректны, повторите попытку\t");
		 error = getint(code);		//получение кол-ва с клавиатуры с клавиатуры
	 }//end while

	 while ((code != 1) && (code != 2) && (code != 3))
	 {
		 printf("Нажмите цифру, которая соответствует необходимой операции: \t");
		 error = getint(code);
	 }//end while
		
	if (code == 1)
	{
		printf("Запущен процесс приема товара\n\n");
		error = SupplyProduct();		//поставка товаров в магазин
		printf("Обновленный список товаров:\n");
		PrintPR();						//печать информации о товарах
	}//end if

	if (code == 2)
	{
		printf("Запущен процесс продажи товара\n\n");
		error = Sale();					//продажа товаров
	}//end if

	if (code == 3)
	{
		printf("Нажмите цифру, которая соответствует необходимой операции \n"
			"4 - вывод базы данных производителей \n\n"
			"5 - вывод базы данных товаров\n\n");
		error = getint(code);

		while (error == INT_MAX)		//пока введенные данные не корректны
		{
			printf("Полученные данные некорректны, повторите попытку\t");
			error = getint(code);		//получение кол-ва с клавиатуры с клавиатуры
		}//end while

		while ((code != 4) && (code != 5))
		{
			printf("Нажмите цифру, которая соответствует необходимой операции: \t");
			error = getint(code);
		}//end while

		if (code == 4)
		{
			printf("Запущен процесс печати данных поставщиков\n\n");
			error = PrintSUP();					//печать базы поставщиков
		}//end if
		else if (code == 5)
		{
			printf("Запущен процесс печати данных товаров\n\n");
			error = PrintPR();					//печать базы товаров
		}//end elae if
	}//end if

	PrintMessage(error);			//печать сообщения о работе программы
	return error;
	
}//end main

/*************************************************************************
*                 Р Е А Л И З А Ц И Я    Ф У Н К Ц И Й                   *
*************************************************************************/

/*=======================================================================/
/ Поставка товара								                         /
/=======================================================================*/
int SupplyProduct()
{
	FILE* base = NULL;					//база данных продуктов
	int k = 0;							//код жля начала записи новой информации
	int code;							//введенное ID
	int error = 0;						//код ошибки
	int q;								//кол-во поступившего товара
	int exit = -1;						//код для выхода из цикла

	base = fopen(PRBASE, "rb");			//открытие файла с продуктами для чтения

	//проверка наличия файлам
	if (!base)							//файла нет в директории проекта
	{
		return -1;						//выход по ошибке
	}//end if

	while (exit != 0)
	{	
		printf("\n");
		PrintSUP();						//печать базы данных поставщиков

		printf("\nУкажите ID существующего поставщика\n"
		"ИЛИ\n"
		"Нажмите -4 для записи нового поставщика:\t");
		
		do{
			error = getint(k);			//получение кода с клавиатуры
			while (error == INT_MAX)	//пока введенные данные не корректны
			{
				printf("\nВы ввели некорректный ID поставщика.\n"
					"Повторите попытку.\t");
				error = getint(k);		//получение кода с клавиатуры
			}//end while

			if (k == -4)				//если код операции соответствует введению нового поставщика
			{
				printf("Укажите ID нового поставщика:\t");
				error = getint(code);	//получение ID с клавиатуры
				while (error == INT_MAX)//пока введенные данные не корректны
				{
					printf("Вы ввели некорректный ID поставщика.\n"
						"Повторите попытку.\t");
					error = getint(code);//получение кода с клавиатуры
				}//end while

				error = Writer("postID", code);//Запись в файл нового поставщика
				if (error != 0)			//если произошла ошибка
				{
					return error;		//вернуть код ошибки
				}//end if
				break;
			}//end if
			else
			{
				code = k;
			}// else

			error = Existence("postID", code);//проверка существования в базе введенного ID поставщика
			if (error == 0)				//если в файле не найден введенный ID
			{
				printf("Вы ввели несуществующий ID поставщика.\n"
					"Повторите попытку.\n"
					"Или нажмите -4 для того чтобы вписать введенный код в список поставщиков как новый\t");
			}//end if
		} while (error == 0);			//пока поставщик с таким кодом не найден

		//если поставщик с таким ID найден
		PrintPR();						//печать базы данных товаров

		printf("\nУкажите код поступившего товара\n"
			"ИЛИ\n"
		"Нажмите -4 для записи нового товара:\t");
		
		do {
			error = getint(k);			//получение кода с клавиатуры
			while (error == INT_MAX)	//пока введенные данные не корректны
			{
				printf("Вы ввели некорректный ID товара.\n"
					"Повторите попытку.\n");
				error = getint(k);		//получение кода с клавиатуры
			}//end while

			if (k == -4)				//если код операции соответствует введению нового товара
			{
				printf("Укажите ID нового товара:\t");
				error = getint(code);	//получение ID с клавиатуры
				while (error == INT_MAX)//пока введенные данные не корректны
				{
					printf("Вы ввели некорректный ID товара.\n"
						"Повторите попытку.\n");
					error = getint(code);//получение ID с клавиатуры
				}//end while
			
				error = Writer("tovID", code);//Запись в файл нового поставщика
				if (error != 0)			//если произошла ошибка
				{
					return error;		//вернуть код ошибки
				}//end if
				break;
			}//end if
			else
			{
				code = k;
			}//end else

			error = Existence("tovID", code);//проверка существования в базе введенного ID товара
			if (error == 0)				//если в файле не найден введенный ID
			{
				printf("Вы ввели несуществующий ID товара.\n"
					"Повторите попытку.\n"
					"Или нажмите -4 для того чтобы вписать введенный код в список товаров как новый\n");
			}//end if
		} while (error == 0);			//пока товар с таким кодом не найден

		if (k != -4)					//если код операции соответствует введению нового товара
		{
			//если товар с таким кодом найден
			printf("Укажите кол-во поступившего товара\t");
			error = getint(q);			//получение кол-ва с клавиатуры
			while (error == INT_MAX)	//пока введенные данные не корректны
			{
				printf("Полученные данные некорректны, повторите попытку\t");
				error = getint(q);		//получение кол-ва с клавиатуры с клавиатуры
			}//end while

			while (q <= 0)				//пока введено отрицательное кол-во товара
			{
				printf("Введено некорректное кол-во поступившего товара. Повторите попытку.\t");
				getint(q);				//получение кол-ва с клавиатуры
			}//end while

			error = ChangeQuantity("+", code, q);//увеличение кол-ва товаров 
			if (error != 0)				//если произошла ошибка
			{
				return error;			//вернуть код ошибки
			}//end if

		}//end if
		printf("\nНажмите 0, если хотите закончить запись поставленных товаров, если хотите продолжить нажмите -1\t");
		error = getint(exit);			//получение кода с клавиатуры с клавиатуры
		while (error == INT_MAX) //пока введенные данные не корректны
		{
			printf("Полученные данные некорректны, повторите попытку\t");
			error = getint(exit);		//получение кода с клавиатуры
		}//end while
		while ((exit != 0) && (exit != -1))
		{
			printf("Нажмите цифру, которая соответствует необходимой операции: \t");
			error = getint(exit);
		}
	}//end while
	return 0;
}//end SupplyProduct

/*=======================================================================/
/ Продажа товара								                         /
/=======================================================================*/
int Sale()
{
	FILE* base = NULL;					//база данных продуктов
	int k = 0;							//код жля начала записи новой информации
	int code;							//введенное ID
	int error;							//код ошибки
	int q;								//кол-во поступившего товара
	int exit = -1;						//код для выхода из цикла
	long weight;						//размер файла
	int place;

	base = fopen(PRBASE, "rb");			//открытие файла с продуктами для чтения

	//проверка наличия файлам
	if (!base)							//файла нет в директории проекта
	{
		return -1;						//выход по ошибке
	}//end if

	fseek(base, 0, SEEK_END);
	weight = ftell(base);
	rewind(base);

	//проверка наличия файлам
	if (weight == 0)					//файл пустой
	{
		return -5;						//выход по ошибке
	}//end if

	fclose(base);						//закрыть файл

	while (exit != 0)
	{
		PrintPR();						//печать базы данных товаров
		printf("Укажите код продаваемого товара\t");
		
		error = getint(code);			//получение кода с клавиатуры
		while (error == INT_MAX)		//пока введенные данные не корректны
		{
			printf("Полученные данные некорректны, повторите попытку\t");
			error = getint(code);		//получение кол-ва с клавиатуры с клавиатуры
		}//end while

		place = Existence("tovID", code);
		while (place == 0)//пока такой код не найден
		{	
			printf("Вы ввели несуществующий ID товара.\n"
				"Повторите попытку.\t");
			error = getint(code);		//получение кода с клавиатуры с клавиатуры	
			place = Existence("tovID", code);
		}//end while
		
		printf("Укажите необходимое количество товара\t");
		error = getint(q);				//получение ко-ва с клавиатуры

		while ((q <= 0) || (error == INT_MAX))//пока введенные данные не корректны
		{
			printf("Введено некорректное кол-во поступившего товара. Повторите попытку.\t");
			error = getint(q);					//получение ко-ва с клавиатуры
		}//end while

		error = CheckQuantity(code, q);	//проверка наичия необходимого кол-ва товара
		if (error == -1)				//если товара нет в наличии
		{
			printf("Данного товара нет в наличии.\t");
			continue;
		}//end if
		else if (error == -4)			//если имеющееся кол-во товара меньше необходимого
		{
			printf("На складе нет необходимого кол-ва товара.\t");
			continue;
		}//end else if
		
		//если необходимое кол-во <= кол-ва в наличии
		error = ChangeQuantity("-", code, q);//уменьшаем кол-во товара
		if (error != 0)					//если товара нет в наличии
		{
			return error;				//вернуть код ошибки
		}//end if

		error = CopyTocart(code, q, exit);	//добавление торава в "корзину"
		if (error != 0)					//если товара нет в наличии
		{
			return error;				//вернуть код ошибки
		}//end if

		printf("Нажмите 0, если хотите закончить запись продаваемых товаров, если хотите продолжить нажмите -2\n");
		error = getint(exit);			//получение кода с клавиатуры
		while (error == INT_MAX)		//пока введенные данные не корректны
		{
			printf("Полученные данные некорректны, повторите попытку\t");
			error = getint(exit);		//получение кода с клавиатуры
		}//end while
		while ((exit != 0) && (exit != -2))
		{
			printf("Нажмите цифру, которая соответствует необходимой операции: \t");
			error = getint(exit);
		}
	}//end while

	error = Bill();						//печать чека
	if (error != 0)						//если произошла ошибка
	{
		return error;					//вернуть код ошибки
	}//end if
}//end Sale

/*=======================================================================/
/ Печать данных из файла с продуктами			                         /
/=======================================================================*/
int PrintPR()
{
	FILE* base = NULL;					//база данных продуктов
	struct product_inf p { 0, {}, 0, 0, 0, {}, 0 };//структура информации о товарах
	int error;							//код ошибки
	long weight;						//размер файла
	void* ptr = NULL;

	base = fopen(PRBASE, "rb");			//открытие файла с продуктами для чтения

	//проверка наличия файлам
	if (!base)							//файла нет в директории проекта
	{
		return -1;						//выход по ошибке
	}//end if

	fseek(base, 0, SEEK_END);
	weight = ftell(base);
	rewind(base);

	//проверка наличия файлам
	if (weight == 0)					//файл пустой
	{
		return -5;						//выход по ошибке
	}//end if

	printf("\n%-*s%-*s%-*s%-*s\n", MAXPR + 5, "Код и название товара", 5 + MAXSUP, "Код и название производителя", 7, "Кол-во", 6, "Цена");

	while (weight > ftell(base))		//пока не конец файла
	{
		ptr = &p.ID_P;
		error = fread(ptr, sizeof(int), 1, base);//чтение ID товара
		if (error < 1)					//произошла ошибка
		{
			return -2;					//выход по ошибке
		}//end if
		
		ptr = &p.pr_name;				
		error = fread(ptr, sizeof(char), MAXPR, base);//чтение имени товара
		if (error < MAXPR)				//произошла ошибка
		{
			return -2;					//выход по ошибке
		}//end if

		ptr = &p.quantity;
		error = fread(ptr, sizeof(int), 1, base);//чтение кол-ва товара
		if (error < 1)					//произошла ошибка
		{
			return -2;					//выход по ошибке
		}//end if

		ptr = &p.price;
		error = fread(ptr, sizeof(int), 1, base);//чтение цены товара
		if (error < 1)					//произошла ошибка
		{
			return -2;					//выход по ошибке
		}//end if

		ptr = &p.ID_S;
		error = fread(ptr, sizeof(int), 1, base);//чтение ID поставщика
		if (error < 1)					//произошла ошибка
		{
			return -2;					//выход по ошибке
		}//end if

		ptr = &p.sup_name;
		error = fread(ptr, sizeof(char), MAXSUP, base);//чтение имени поставщика товара
		if (error < MAXSUP)				//произошла ошибка
		{
			return -2;					//выход по ошибке
		}//end if

		//печать информации о товарах
		printf("%0*d %-*s%-*d %-*s%-*d%-*d\n", 4, p.ID_P, MAXPR, p.pr_name, 4, p.ID_S, MAXSUP, p.sup_name, 6, p.quantity, 6, p.price);
	}//end while

	fclose(base);						//закрыть файл
	return 0;
}//end PrintPR

/*=======================================================================/
/ Печать данных из файла с продуктами			                         /
/=======================================================================*/
int PrintSUP()
{
	FILE* base = NULL;					//база данных поставщиков
	struct product_inf p = { 0,{},0,0,0,{},0 };//структура информации о товарах
	int error;							//код ошибки
	long weight;						//размер файла
	void* ptr = NULL;

	base = fopen(SUPBASE, "rb");		//открытие файла с продуктами для чтения

	//проверка наличия файлам
	if (!base)							//файла нет в директории проекта
	{
		return -1;						//выход по ошибке
	}//end if

	fseek(base, 0, SEEK_END);
	weight = ftell(base);
	rewind(base);

	//проверка наличия файлам
	if (weight == 0)					//файл пустой
	{
		return -5;						//выход по ошибке
	}//end if

	//печать заголовка списка
	printf("%-*s\n", 5 + MAXSUP, "Код и название производителя");

	while (weight > ftell(base))		//пока не конец файла
	{
		ptr = &p.ID_S;
		error = fread(ptr, sizeof(int), 1, base);//чтение ID поставщика
		if (error < 1)					//произошла ошибка
		{
			return -2;					//выход по ошибке
		}//end if

		ptr = &p.sup_name;
		error = fread(ptr, sizeof(char), MAXSUP, base);//чтение имени поставщика товара
		if (error < MAXSUP)				//произошла ошибка
		{
			return -2;					//выход по ошибке
		}//end if

		//печать информации о поставщиках
		printf("%0*d %-*s\n", 4, p.ID_S, MAXSUP, p.sup_name);
	}//end while

	fclose(base);						//закрыть файл
	return 0;
}//end PrintSUP

/*=======================================================================/
/ Проверка существования производителя или товара                        /
/=======================================================================*/
int Existence(const char(*key), int ID)
{
	FILE* base = NULL;					//база данных
	struct product_inf p = { 0,{},0,0,0,{},0 };//структура информации о товарах
	int error = 0;						//код ошибки
	int foundID;						//найденный ID товара
	long weight;						//размер файла
	void* ptr = NULL;
	
	//инициализация файловых переменных
	if (key == "postID")
	{
		base = fopen(SUPBASE, "rb");	//открыть файл данных о поставщиках для чтения
	}//end if
	else if (key == "tovID")
	{
		base = fopen(PRBASE, "rb");		//открыть файл данных о товарах для чтения
	}//end if

	//проверка наличия файлам
	if (!base)							//файла нет в директории проекта
	{
		return -1;						//выход по ошибке
	}//end if

	fseek(base, 0, SEEK_END);
	weight = ftell(base);
	rewind(base);

	//проверка наличия данных в файле
	if (weight == 0)					//файл пустой
	{
		return -5;						//выход по ошибке
	}//end if

	while (weight > ftell(base))		//пока не конец файла
	{
		ptr = &foundID;
		error = fread(ptr, sizeof(int), 1, base);//чтение текущего ID из файла
		if (error <= 0)					//произошла ошибка чтения
		{
			fclose(base);				//закрыть файл
			return -2;					//выход по ошибке
		}//end if

		if (foundID == ID)				//текущий ID совпадает с введенным
		{	
			p.place = ftell(base);		//запомнить место строки в файле 
			return p.place;					//выход по ошибке
		}//end if
		else
		{
			if (key == "postID")		//если работа с файлом поставщиков
			{
				fseek(base, MAXSUP * sizeof(char), SEEK_CUR);//пропуск информации в строке
			}//end if
			else if (key == "tovID")	//если работа с файлом товаров
			{
				fseek(base, (MAXPR + MAXSUP) * sizeof(char) + 3 * sizeof(int) , SEEK_CUR);//пропуск информации в строке
			}//end else if
		}//end else
	}//end while

	fclose(base);						//закрыть файл
	return 0;
}//end Existence

/*=======================================================================/
/ Запись нового производителя или товара в файл                          /
/=======================================================================*/
int Writer(const char(*key), int ID)
{
	FILE* base = NULL;					//база данных
	struct product_inf p= { 0,{},0,0,0,{},0 };//структура информации о товарах
	int error;							//код ошибки
	void* c = NULL;
	char s[LEN] = "";

	if (key == "postID")
	{
		base = fopen(SUPBASE, "a+b");	//открыть файл данных о поставщиках для дозаписи
	}//end if
	else if (key == "tovID")
	{
		base = fopen(PRBASE, "a+b");	//открыть файл данных о товарах для дозаписи
	}//end else if

	//проверка наличия файлам
	if (!base)//файла нет в директории проекта
	{
		return -1;						//выход по ошибке
	}//end if

	if (key == "postID")				//если работаем с файлом поставщиков
	{
		p.ID_S = ID;
		c = &p.ID_S;
		error = fwrite(c, sizeof(int), 1, base);//запись в файл поставщиков ID нового поставщика
		if (error < 1)					//если произошла ошибка
		{
			return -3;					//выход по ошибке
		}//end if

		printf("Укажите название поставщика\t");
		gets_s(p.sup_name);				//получение имя нового поставшика
		sprintf(s, "%-*s", MAXSUP, p.sup_name);//дополнение имени пробелами до максимальной длины 
		c = &s;
		error = fwrite(c, sizeof(char), MAXSUP, base);//запись нового имени в файл
		if (error < MAXSUP)				//если произошла ошибка
		{
			return -3;					//выход по ошибке
		}//end if
	}//end if
	else if (key == "tovID")			//если работаем с файлом поставщико
	{
		p.ID_P = ID;
		c = &p.ID_P;
		error = fwrite(c, sizeof(int), 1, base);//запись в файл товаров ID нового товара
		if (error < 1)					//если произошла ошибка
		{
			return -3;					//выход по ошибке
		}//end if
		
		printf("Укажите название поступившего товара\t");
		gets_s(p.pr_name);				//получение имя нового товара
		sprintf(s, "%-*s", MAXPR, p.pr_name);//дополнение имени пробелами до максимальной длины
		c = &s;
		error = fwrite(c, sizeof(char), MAXPR, base);//запись нового имени в файл
		if (error < MAXPR)				//если произошла ошибка
		{
			return -3;					//выход по ошибке
		}//end if

		printf("Укажите количество поступившего товара\t");
		error = getint(p.quantity);		//получение кол-ва нового товара
		while (error == INT_MAX)	//пока введенные данные не корректны
		{
			printf("Полученные данные некорректны, повторите попытку\t");
			error = getint(p.quantity);		//получение кол-ва с клавиатуры
		}//end while
		c = &p.quantity;
		error = fwrite(c, sizeof(int), 1, base);//запись кол-ва в файл
		if (error < 1)					//если произошла ошибка
		{
			return -3;					//выход по ошибке
		}//end if

		printf("Укажите цену поступившего товара\t");
		error = getint(p.price);			//получение цены нового товара
		while (error == INT_MAX)			//пока введенные данные не корректны
		{
			printf("Полученные данные некорректны, повторите попытку\t");
			error = getint(p.price);		//получение кол-ва с клавиатуры
		}//end while
		c = &p.price;
		error = fwrite(c, sizeof(int), 1, base);//запись цены в файл
		if (error < 1)					//если произошла ошибка
		{
			return -3;					//выход по ошибке
		}//end if

		printf("Укажите код производителя\t");
		error = getint(p.ID_S);					//получение ID производителя нового товара
		while (error == INT_MAX)	//пока введенные данные не корректны
		{
			printf("Полученные данные некорректны, повторите попытку\t");
			error = getint(p.ID_S);		//получение кол-ва с клавиатуры
		}//end while
		c = &p.ID_S;
		error = fwrite(c, sizeof(int), 1, base);//запись цены в файл
		if (error < 1)					//если произошла ошибка
		{
			return -3;					//выход по ошибке
		}//end if

		printf("Укажите название производителя\t");
		gets_s(p.sup_name);
		sprintf(s, "%-*s", MAXSUP, p.sup_name);//получение ID производителя нового товара
		c = &s;
		error = fwrite(c, sizeof(char), MAXSUP, base);//запись цены в файл
		if (error < MAXSUP)				//если произошла ошибка
		{
			return -3;					//выход по ошибке
		}//end if
		
	}//end else if

	fclose(base);						//закрыть файл
	return 0;
}//end Writer

/*=======================================================================/
/ Проверка наличия необходимого количества товара                        /
/=======================================================================*/
int CheckQuantity(int ID, int quantity)
{
	FILE* base = NULL;					//база данных
	struct product_inf p = { 0,{},0,0,0,{},0 };//структура информации о товарах
	int error;							//код ошибки
	long weight;						//размер файла
	int foundkol;						//кол-во товара в файле
	void* ptr = NULL;

	base = fopen(PRBASE, "rb");			//открыть файл данных о товарах для чтения

	//проверка наличия файлам
	if (!base)							//файла нет в директории проекта
	{
		return -1;						//выход по ошибке
	}//end if

	fseek(base, 0, SEEK_END);
	weight = ftell(base);
	rewind(base);

	//проверка наличия данных в файле
	if (weight == 0)					//файл пустой
	{
		return -5;						//выход по ошибке
	}//end if

	fseek(base, p.place, SEEK_SET);		//пропустить все данные до места необходимого товара
	fseek(base, MAXPR * sizeof(char)+ sizeof(int), SEEK_CUR);//пропустить название товара и ID

	ptr = &foundkol;
	error = fread(ptr, sizeof(int), 1, base);//чтение из файла кол-ва товара
	if (error < 1)						//произощла ошибка
	{
		return -2;						//выход по ошибке
	}//end if

	if (foundkol >= quantity)			//найденное кло-во больше или равно необхаодимому
	{
		return 0;
	}//end if
	else if (foundkol == 0)				//товара нет в наличии
	{
		return -1;						//выход по ошибке
	}//end else if
	else if (foundkol < quantity)		//найденное кло-во меньше или равно необхаодимому
	{
		return -4;						//выход по ошибке
	}//end else if

	fclose(base);						//закрыть файл
}//end CheckQuantity

/*=======================================================================/
/ Изменение количества товара при поставке и продаже                     /
/=======================================================================*/
int ChangeQuantity(const char(*key), int ID, int quantity)
{
	FILE* base = NULL;					//база данных
	struct product_inf p = { 0,{},0,0,0,{},0 };//структура информации о товарах
	int q;								//кол-во товара в файле
	int error;							//код ошибки
	long weight;						//размер файла
	void* ptr = NULL;
	
	base = fopen(PRBASE, "rb");			//открыть файл данных о товарах для чтения

	//проверка наличия файлам
	if (!base)							//файла нет в директории проекта
	{
		return -1;						//выход по ошибке
	}//end if

	fseek(base, 0, SEEK_END);
	weight = ftell(base);
	rewind(base);

	//проверка наличия данных в файле
	if (weight == 0)					//файл пустой
	{
		return -5;						//выход по ошибке
	}//end if

	fseek(base, p.place, SEEK_SET);		//пропустить все данные до места необходимого товара
	fseek(base, MAXPR * sizeof(char) + sizeof(int), SEEK_CUR);//пропустить название товара и ID

	ptr = &q;
	error = fread(ptr, sizeof(int), 1, base);//прочитать кол-во товара ф файле
	if (key == "+")
	{
		q = quantity;
	}//end if
	else if (key == "-")
	{
		q = -quantity;
	}//end else if

	ptr = &q;

	fclose(base);						//закрыть файл

	error = CopyChange("1", ID, q);		//перенести данные из одного файла в другой
	if (error != 0)						//если произошла ошибка
	{
		return error;					//веруть код ошибки
	}//end if

	error = CopyChange("2", ID, 0);		//перенести данные из одного файла в другой
	if (error != 0)						//если произошла ошибка
	{
		return error;					//веруть код ошибки
	}//end if

	return 0;
}//end ChangeQuantity

/*=======================================================================/
/ Вывод счета на купленный товар			                             /
/=======================================================================*/
int Bill()
{
	FILE* cart = NULL;					//база данных продуктов, которые будут напечатаны в чек
	struct product_inf p = { 0, {}, 0, 0, 0, {}, 0 };//структура информации о товарах
	tm date = { 0,0,0,0,0,0,0,0,0 };	//структура даты
	int error;							//код ошибки
	long weight;						//размер файла
	int sum = 0;						//сумма покупки
	void* ptr = NULL;
	
	date = today();						//определение сегодняшней даты

	cart = fopen(CART, "rb");			//открыть файл с покупками для чтения

	//проверка наличия файлам
	if (!cart)							//файла нет в директории проекта
	{
		return -1;						//выход по ошибке
	}//end if

	fseek(cart, 0, SEEK_END);
	weight = ftell(cart);
	rewind(cart);

	//проверка наличия данных в файле
	if (weight == 0)					//файл пустой
	{
		return -5;						//выход по ошибке
	}//end if

	//печать "шапки" чека и текущей даты
	printf("\t\tКассовый чек\nООО Мой магазин аудио-видео техники\n%d.%d.%d\n", date.tm_mday, date.tm_mon, date.tm_year);
	
	while (weight > ftell(cart))		//пока не конец файла
	{
		ptr = &p.ID_P;
		error = fread(ptr, sizeof(int), 1, cart);//чтение ID товара
		if (error <= 0)					//произошла ошибка
		{
			fclose(cart);				//закрыть файл
			return -2;					//выход по ошибке
		}//end if

		ptr = &p.pr_name;
		error = fread(ptr, sizeof(char), MAXPR, cart);//чтение имени товара
		if (error <	MAXPR)					//произошла ошибка
		{
			fclose(cart);				//закрыть файл
			return -2;					//выход по ошибке
		}//end if

		ptr = &p.quantity;
		error = fread(ptr, sizeof(int), 1, cart);//чтение кол-ва товара
		if (error <= 0)					//произошла ошибка
		{
			fclose(cart);				//закрыть файл
			return -2;					//выход по ошибке
		}//end if

		ptr = &p.price;
		error = fread(ptr, sizeof(int), 1, cart);//чтение цены товара
		if (error <= 0)					//произошла ошибка
		{
			fclose(cart);				//закрыть файл
			return -2;					//выход по ошибке
		}//end if

		fseek(cart, MAXSUP * sizeof(char) + sizeof(int), SEEK_CUR);//пропустить название товара и ID

		sum += p.quantity * p.price;	//подсчет суммы покупки

		//печать информации о товарах в чек
		printf("%s\t %dшт x %dруб\t = %d рублей\n", p.pr_name, p.quantity, p.price, p.quantity * p.price);
	}//end while

	printf("ИТОГ:\t%d рублей", sum);			//печать суммы покупки

	fclose(cart);						//закрыть файл
	return 0;
}//end Bill

/*=======================================================================/
/ Определение текущей даты						                         /
/=======================================================================*/
tm today()
{
	tm* date;
	time_t current;

	time(&current);
	date = localtime(&current);
	date->tm_year += 1900;
	date->tm_mon += 1;

	return *date;
}//end today

/*=======================================================================/
/ Получение числа с клавиатуры					                         /
/=======================================================================*/
int getint(int& digit)
{
	char string[LEN] = "";
	char* word = NULL;
	
	gets_s(string);
	word = strtok(string, "\t ");
	if (word == NULL)
	{
		digit = -3;
		return INT_MAX;
	}//end if
	if (word[0] == '0')
	{
		digit = 0;
		return 0;
	}//end if

	digit = atoi(word);
	if (digit == 0)
	{
		digit = -3;
		return INT_MAX;
	}//end if

	return 0;
}//end getint

/*=======================================================================/
/ Печать данных из одного файла в другой		                         /
/=======================================================================*/
int CopyChange(const char(*key), int ID, int quantity)
{
	FILE* base = NULL;					//файл из которого копируют информацию
	FILE* copy = NULL;					//файл в который копируют информацию
	struct product_inf p = { 0,{},0,0,0,{},0 };//структура информации о товарах
	long weight;						//размер файла
	int error;							//код ошибки
	void* ptr = NULL;
	char s[LEN] = "";

	if (key == "1")
	{
		base = fopen(PRBASE, "rb");		//открытие файла с продуктами для чтения

		//проверка наличия файлам
		if (!base)						//файла нет в директории проекта
		{
			return -1;					//выход по ошибке
		}//end if

		copy = fopen(COPY, "wb");		//открытие файла, в который будет производится копирование, для записи
		
		//проверка наличия файлам
		if (!copy)						//файла нет в директории проекта
		{
			return -1;					//выход по ошибке
		}//end if
	}//end if
	else if (key == "2")
	{
		base = fopen(COPY, "rb");		//открытие файла, в который было производено копирование, для чтения
		if (!base)						//файла нет в директории проекта
		{
			return -1;					//выход по ошибке
		}//end if

		copy = fopen(PRBASE, "wb");		//открытие файла с продуктами для записи
		if (!copy)						//файла нет в директории проекта
		{
			return -1;					//выход по ошибке
		}//end if
	}//end else if

	fseek(base, 0, SEEK_END);
	weight = ftell(base);
	rewind(base);

	//проверка наличия файлам
	if (weight == 0)					//файл пустой
	{
		return -5;						//выход по ошибке
	}//end if

	while (weight > ftell(base))		//пока не конец файла
	{
		ptr = &p.ID_P;
		error = fread(ptr, sizeof(int), 1, base);//чтение ID товара
		if (error <= 0)					//произошла ошибка
		{
			fclose(base);				//закрыть файл
			fclose(copy);				//закрыть файл
			return -2;					//выход по ошибке
		}//end if
		error = fwrite(ptr, sizeof(int), 1, copy);//запись ID товара
		if (error < 1)					//произошла ошибка
		{
			fclose(base);				//закрыть файл
			fclose(copy);				//закрыть файл
			return -3;					//выход по ошибке
		}//end if

		ptr = &p.pr_name;
		error = fread(ptr, sizeof(char), MAXPR, base);//чтение имени товара
		if (error <= 0)					//произошла ошибка
		{
			fclose(base);				//закрыть файл
			fclose(copy);				//закрыть файл
			return -2;					//выход по ошибке
		}//end if
		sprintf(s, "%-*s", MAXPR, p.pr_name);//дополнение имени пробелами до максимальной длины
		ptr = &s;
		error = fwrite(ptr, sizeof(char), MAXPR, copy);//запись имени товара
		if (error < MAXPR)				//произошла ошибка
		{
			fclose(base);				//закрыть файл
			fclose(copy);				//закрыть файл
			return -3;					//выход по ошибке
		}//end if

		ptr = &p.quantity;
		error = fread(ptr, sizeof(int), 1, base);//чтение кол-ва товара
		if (error <= 0)					//произошла ошибка
		{
			fclose(base);				//закрыть файл
			fclose(copy);				//закрыть файл
			return -2;					//выход по ошибке
		}//end if

		//если нашли продаваемый товар, то структуре присваиваем измененные кол-во
		if (ID == p.ID_P) 
		{
			p.quantity += quantity;		//изменение кол-ва товара
		}//end if

		error = fwrite(ptr, sizeof(int), 1, copy);//запись кол-ва товара
		if (error < 1)					//произошла ошибка
		{
			fclose(base);				//закрыть файл
			fclose(copy);				//закрыть файл
			return -3;					//выход по ошибке
		}//end if

		ptr = &p.price;
		error = fread(ptr, sizeof(int), 1, base);//чтение цены товара
		if (error <= 0)					//произошла ошибка
		{
			fclose(base);				//закрыть файл
			fclose(copy);				//закрыть файл
			return -2;					//выход по ошибке
		}//end if
		error = fwrite(ptr, sizeof(int), 1, copy);//запись цены товара
		if (error < 1)					//произошла ошибка
		{
			fclose(base);				//закрыть файл
			fclose(copy);				//закрыть файл
			return -3;					//выход по ошибке
		}//end if

		ptr = &p.ID_S;
		error = fread(ptr, sizeof(int), 1, base);//чтение ID поставщика
		if (error <= 0)					//произошла ошибка
		{
			fclose(base);				//закрыть файл
			fclose(copy);				//закрыть файл
			return -2;					//выход по ошибке
		}//end if
		error = fwrite(ptr, sizeof(int), 1, copy);//запись ID поставщика
		if (error < 1)					//произошла ошибка
		{
			fclose(base);				//закрыть файл
			fclose(copy);				//закрыть файл
			return -3;					//выход по ошибке
		}//end if

		ptr = &p.sup_name;
		error = fread(ptr, sizeof(char), MAXSUP, base);//чтение имени поставщика товара
		if (error <= 0)					//произошла ошибка
		{
			fclose(base);				//закрыть файл
			fclose(copy);				//закрыть файл
			return -2;					//выход по ошибке
		}//end if
		sprintf(s, "%-*s", MAXSUP, p.sup_name);//дополнение имени пробелами до максимальной длины
		ptr = &s;
		error = fwrite(ptr, sizeof(char), MAXSUP, copy);//запись имени поставщика товара
		if (error < MAXSUP)				//произошла ошибка
		{
			fclose(base);				//закрыть файл
			fclose(copy);				//закрыть файл
			return -3;					//выход по ошибке
		}//end if
	}//end while

	fclose(base);						//закрыть файл
	fclose(copy);						//закрыть файл
	return 0;
}//end CopyChange

/*=======================================================================/
/ Печать определенной строки из одного файла в другой		             /
/=======================================================================*/
int CopyTocart(int ID, int quantity, int code)
{	
	FILE* cart = NULL;					//"корзина" с продуктами
	FILE* base = NULL;					//база данных с продуктами
	struct product_inf p = { 0,{},0,0,0,{},0 };//структура информации о товарах
	long weight;						//размер файла
	int error;							//код ошибки
	int foundID;						//найденный ID товара
	void* ptr = NULL;
	char s[LEN] = "";
	
	base = fopen(PRBASE, "rb");

	//проверка наличия файлам
	if (!base)							//файла нет в директории проекта
	{
		return -1;						//выход по ошибке
	}//end if

	if (code == -1)
	{
		cart = fopen(CART, "wb");
	}
	else
	{
		cart = fopen(CART, "a+b");
	}

	//проверка наличия файлам
	if (!cart)							//файла нет в директории проекта
	{
		return -1;						//выход по ошибке
	}//end if

	fseek(base, 0, SEEK_END);
	weight = ftell(base);
	rewind(base);

	//проверка наличия файлам
	if (weight == 0)
	{
		return -5;						//выход по ошибке
	}//end if

	while (weight > ftell(base))		//пока не конец файла
	{
		ptr = &foundID;
		error = fread(ptr, sizeof(int), 1, base);//чтение текущего ID из файла
		if (error <= 0)					//произошла ошибка чтения
		{
			fclose(base);				//закрыть файл
			return -2;					//выход по ошибке
		}//end if

		if (foundID == ID)				//текущий ID совпадает с введенным
		{
			ptr = &ID;
			error = fwrite(ptr, sizeof(int), 1, cart);//запись ID товара
			if (error < 1)						//произошла ошибка
			{
				fclose(base);					//закрыть файл
				fclose(cart);					//закрыть файл
				return -3;						//выход по ошибке
			}//end if

			ptr = &p.pr_name;
			error = fread(ptr, sizeof(char), MAXPR, base);//чтение имени товара
			if (error <= 0)						//произошла ошибка
			{
				fclose(base);					//закрыть файл
				fclose(cart);					//закрыть файл
				return -2;						//выход по ошибке
			}//end if
			sprintf(s, "%-*s", MAXPR, p.pr_name);//дополнение имени пробелами до максимальной длины
			ptr = &s;
			error = fwrite(ptr, sizeof(char), MAXPR, cart);//запись имени товара
			if (error < MAXPR)					//произошла ошибка
			{
				fclose(base);					//закрыть файл
				fclose(cart);					//закрыть файл
				return -3;						//выход по ошибке
			}//end if

			ptr = &quantity;
			error = fwrite(ptr, sizeof(int), 1, cart);//чтение кол-ва товара
			if (error < 1)						//произошла ошибка
			{
				fclose(base);					//закрыть файл
				fclose(cart);					//закрыть файл
				return -3;						//выход по ошибке
			}//end if

			fseek(base, sizeof(int), SEEK_CUR);

			ptr = &p.price;
			error = fread(ptr, sizeof(int), 1, base);//чтение цены товара
			if (error <= 0)						//произошла ошибка
			{
				fclose(base);					//закрыть файл
				fclose(cart);					//закрыть файл
				return -2;						//выход по ошибке
			}//end if
			error = fwrite(ptr, sizeof(int), 1, cart);//чтение цены товара
			if (error < 1)						//произошла ошибка
			{
				fclose(base);					//закрыть файл
				fclose(cart);					//закрыть файл
				return -3;						//выход по ошибке
			}//end if

			ptr = &p.ID_S;
			error = fread(ptr, sizeof(int), 1, base);//чтение ID поставщика
			if (error <= 0)						//произошла ошибка
			{
				fclose(base);					//закрыть файл
				fclose(cart);					//закрыть файл
				return -2;						//выход по ошибке
			}//end if
			error = fwrite(ptr, sizeof(int), 1, cart);//запись ID поставщика
			if (error < 1)						//произошла ошибка
			{
				fclose(base);					//закрыть файл
				fclose(cart);					//закрыть файл
				return -3;						//выход по ошибке
			}//end if

			ptr = &p.sup_name;
			error = fread(ptr, sizeof(char), MAXSUP, base);//чтение имени поставщика товара
			if (error <= 0)						//произошла ошибка
			{
				fclose(base);					//закрыть файл
				fclose(cart);					//закрыть файл
				return -2;						//выход по ошибке
			}//end if
			sprintf(s, "%-*s", MAXSUP, p.sup_name);//дополнение имени пробелами до максимальной длины
			ptr = &s;
			error = fwrite(ptr, sizeof(char), MAXSUP, cart);//запись имени поставщика товара
			if (error < MAXSUP)					//произошла ошибка
			{
				fclose(base);					//закрыть файл
				fclose(cart);					//закрыть файл
				return -3;						//выход по ошибке
			}//end if

		}//end if
		else
		{
				fseek(base, (MAXPR + MAXSUP) * sizeof(char) + 3 * sizeof(int), SEEK_CUR);//пропуск информации в строке
		}//end else
	}//end while
	
			fclose(base);						//закрыть файл
			fclose(cart);						//закрыть файл
	return 0;
}//end CopyTocart

/*=======================================================================/
/ Вывод сообщений о результате работы программы                          /
/=======================================================================*/
void PrintMessage(int error_code)
{
	//печать сообщения по коду ошибки
	switch (error_code)
	{
	case 0:
		printf("\nОбработка данных из файла завершена успешно.\n");
		break;

	case -1:
		printf("Файл не найден.\n");
		break;

	case -2:
		printf("Ошибка чтения файла.\n");
		break;

	case -3:
		printf("Ошибка записи в файл.\n");
		break;

	case -5:
		printf("Файл пустой.\n");
		break;

	}//end switch

}//end PrinMessage()