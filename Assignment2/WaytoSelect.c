if (randi2 == 9)
			{
				flag2 = 0;
				countInvalid++;
			}
			else if (randi1>=0 && randi1< 2 && randi2 !=9)
			{
				network1 = networkA[2];
				if (randi2>=0 && randi2 <2)
				{
					network2 = networkA[2];
					countInvalid++;
				}
				else if (randi2 >=2 && randi2 <5)
				{
					network2 = networkB[3];
					countAtoB++;
				}
				else if (randi2 >=5 && randi2 <8)
				{
					network2 = networkC[4];
					countAtoC++;
				}
				
			}
			else if (randi1 >=2 && randi1 < 5 && randi2 != 9)
			{
				network1 = networkB[3];
				if (randi2>=0 && randi2 <2)
				{
					network2 = networkA[2];
					countBtoA++;
				}
				else if (randi2 >=2 && randi2 <5)
				{
					network2 = networkB[3];
					countInvalid++;
				}
				else if (randi2 >=5 && randi2 <8)
				{
					network2 = networkC[4];
					countBtoC++;
				}
			}
			else if (randi1 >=5 && randi1 < 9 && randi2 != 9)
			{	
				network1 = networkC[4];
				if (randi2>=0 && randi2 <2)
				{
					network2 = networkA[2];
					countCtoA++;
				}
				else if (randi2 >=2 && randi2 <5)
				{
					network2 = networkB[3];
					countCtoB++;
				}
				else if (randi2 >=5 && randi2 <8)
				{
					network2 = networkC[4];
					countInvalid++;
				}
			}
