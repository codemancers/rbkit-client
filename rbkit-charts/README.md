This repo holds the charting-related JavaScript files. The contents
of this repo are added to the main `rbkit-client` repo by the way of Git
subtrees.


To get the latest updates from this repo into the `rbkit-client` projects,
use this following commands:

1. Add a remote pointing to this repo

    git remote add rbkit-charts git@github.com:code-mancers/rbkit-chartjs.git

2. Issue a pull to fetch the new changes

    git subtree pull --prefix /path/to/charts/repo rbkit-charts master --squash
