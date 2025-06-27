# Plaintext Keyword Search - Phase 1

This directory contains Python scripts used in **Phase 1** of the thesis project titled:

> **"Securing Third-Party Data in Forensic Investigations Using Homomorphic Encryption"**

Here, we implement and evaluate **keyword search on plaintext email data** as a baseline for comparison with **homomorphically encrypted search**.

---

## Directory Contents

```
plaintext-scripts/
│
├── keyword_search_1.py # Linear search implementation
├── keyword_search_2.py # Vectorized search using pandas
└── README.md # This file
```

## Dataset

Both scripts operate on a **preprocessed CSV file** located at: [Cleaned Datasets](../../datasets/cleaned/split-1.csv)


Each row of the file contains cleaned Enron emails with the following columns:

- `email_id`
- `subject`
- `body`
- `message` (optional combined field)

---

## How to Use

### 1. Linear Search (Row-by-Row)

```
python3 keyword_search_1.py brenda
```

This script searches each row individually for the keyword in both the subject and body fields.
It prints any matches found, along with:

- Line number
- Email ID
- Date
- Subject
- Snippet of the body
- Total time taken to search

#### Sample Output:

```
Searching for keyword: 'brenda'

9 match(es) found:

Line: 22 | ID: allen-p/_sent_mail/112.
Date: Tue, 3 Oct 2000 09:13:00 -0700 (PDT)
Subject: Mime-Version: 1.0
Body: Brenda, Please use the second check as the October payment. If you have already tossed it, let me know so I can mail you another. Phillip

==========================================================================================

Line: 24 | ID: allen-p/_sent_mail/114.
Date: Mon, 2 Oct 2000 02:19:00 -0700 (PDT)
Subject: Re: Original Sept check/closing
Body: Brenda, Please use the second check as my October payment. I have my copy of the original deal. Do you want me to fax this to you? Phillip

==========================================================================================

Line: 34 | ID: allen-p/_sent_mail/123.
Date: Tue, 26 Sep 2000 04:57:00 -0700 (PDT)
Subject: closing
Body: ---------------------- Forwarded by Phillip K Allen/HOU/ECT on 09/26/2000 11:57 AM --------------------------- "BS Stone" <bs_stone@yahoo.com> on 09/26/2000 04:47:40 AM To: "jeff" <jeff@freeyellow.com> cc: "Phillip K Allen" <Phillip.K.Allen@enron.com> Subject: closing Jeff, ? Is the closing today?? After reviewing the agreement?I find it isn't binding as far as I can determine.? It is too vague and it doesn't sound like anything an attorney or title company would?draft for a real estate closing--but, of course, I could be wrong.? ? If this?closing is going to take place without this agreement then there is no point in me following up on this?document's validity.? ? I will just need to go back to my closing documents and see what's there and find out where I am with that and deal with this as best I can. ? I guess I was expecting something that would be an exhibit to a recordable document or something a little more exact, or rather?sort of a contract.? This isn't either.? I tried to get a real estate atty on the phone last night but he was out of pocket.? I talked to a crim. atty friend and he said this is out of his area but doesn't sound binding to him.? ? I will go back to mine and Phillip Allen's transaction?and take a look at that but as vague and general as this is I doubt that my signature? is even needed to complete this transaction.? I am in after 12 noon if there is any need to contact me regarding the closing. ? I really do not want to hold up anything or generate more work for myself and I don't want to insult or annoy anyone but this paper really doesn't seem to be something required for a closing.? In the event you do need my signature on something like this I would rather have time to have it reviewed before I accept it. ? Brenda ? ?

==========================================================================================

Line: 46 | ID: allen-p/_sent_mail/134.
Date: Mon, 18 Sep 2000 02:34:00 -0700 (PDT)
Subject: Re: burnet
Body: Jeff, I need to see the site plan for Burnet. Remember I must get written approval from Brenda Key Stone before I can sell this property and she has concerns about the way the property will be subdivided. I would also like to review the closing statements as soon as possible. Phillip
```

### 2. Optimized Search (Pandas Vectorized)

```
python3 keyword_search_optimized.py Brenda
```

This uses efficient **pandas string operations** to find all matching rows in a single call.
It provides the same output but is much faster and suitable for larger datasets.

#### Sample Output

```
Searching for keyword: 'brenda'

9 match(es) found in 0.0028 seconds.

Line: 22 | ID: allen-p/_sent_mail/112.
Date: Tue, 3 Oct 2000 09:13:00 -0700 (PDT)
Subject: Mime-Version: 1.0
Body: Brenda, Please use the second check as the October payment. If you have already tossed it, let me know so I can mail you another. Phillip

==========================================================================================

Line: 24 | ID: allen-p/_sent_mail/114.
Date: Mon, 2 Oct 2000 02:19:00 -0700 (PDT)
Subject: Re: Original Sept check/closing
Body: Brenda, Please use the second check as my October payment. I have my copy of the original deal. Do you want me to fax this to you? Phillip

==========================================================================================

Line: 34 | ID: allen-p/_sent_mail/123.
Date: Tue, 26 Sep 2000 04:57:00 -0700 (PDT)
Subject: closing
Body: ---------------------- Forwarded by Phillip K Allen/HOU/ECT on 09/26/2000 11:57 AM --------------------------- "BS Stone" <bs_stone@yahoo.com> on 09/26/2000 04:47:40 AM To: "jeff" <jeff@freeyellow.com> cc: "Phillip K Allen" <Phillip.K.Allen@enron.com> Subject: closing Jeff, ? Is the closing today?? After reviewing the agreement?I find it isn't binding as far as I can determine.? It is too vague and it doesn't sound like anything an attorney or title company would?draft for a real estate closing--but, of course, I could be wrong.? ? If this?closing is going to take place without this agreement then there is no point in me following up on this?document's validity.? ? I will just need to go back to my closing documents and see what's there and find out where I am with that and deal with this as best I can. ? I guess I was expecting something that would be an exhibit to a recordable document or something a little more exact, or rather?sort of a contract.? This isn't either.? I tried to get a real estate atty on the phone last night but he was out of pocket.? I talked to a crim. atty friend and he said this is out of his area but doesn't sound binding to him.? ? I will go back to mine and Phillip Allen's transaction?and take a look at that but as vague and general as this is I doubt that my signature? is even needed to complete this transaction.? I am in after 12 noon if there is any need to contact me regarding the closing. ? I really do not want to hold up anything or generate more work for myself and I don't want to insult or annoy anyone but this paper really doesn't seem to be something required for a closing.? In the event you do need my signature on something like this I would rather have time to have it reviewed before I accept it. ? Brenda ? ?

==========================================================================================

Line: 46 | ID: allen-p/_sent_mail/134.
Date: Mon, 18 Sep 2000 02:34:00 -0700 (PDT)
Subject: Re: burnet
Body: Jeff, I need to see the site plan for Burnet. Remember I must get written approval from Brenda Key Stone before I can sell this property and she has concerns about the way the property will be subdivided. I would also like to review the closing statements as soon as possible. Phillip
```


## Why This Phase Matters
This plaintext search is used to:

- Establish a **performance baseline** for keyword search
- Evaluate **accuracy** (true/false positives)
- Measure **search speed**
- Show why plaintext access is a **privacy risk** in forensic contexts

The next phase performs the same search over encrypted data using Microsoft SEAL and homomorphic encryption techniques.


## Requirements

* Python 3.x
* `pandas` installed

```
pip install pandas
```
